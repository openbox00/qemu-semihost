#include <stdio.h>
#include <inttypes.h>

/* can ref arm website: 
 * http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0471c/CHDGGIID.html
 */
enum SYS_CALL {
	SYS_OPEN	= 0x01,
	SYS_CLOSE	= 0x02,
	SYS_WRITEC	= 0x03,
	SYS_WRITE0	= 0x04,
	SYS_WRITE	= 0x05,
	SYS_READ	= 0x06,
	SYS_READC	= 0x07
};

/* ref rtenv*/
size_t strlen(const char *s) __attribute__ ((naked));
size_t strlen(const char *s)
{
	__asm__ __volatile__(
		"sub  r3, r0, #1			\n"
       		"strlen_loop:               		\n"
		"	ldrb r2, [r3, #1]!		\n"
		"	cmp  r2, #0			\n"
	        " 	bne  strlen_loop        	\n"
		"	sub  r0, r3, r0			\n"
		"	bx   lr				\n"
		:::
	);
}


/*ref zzz00072 & aacczury*/
int simicall(enum SYS_CALL action, void *arg) __attribute__ ((naked));
int simicall(enum SYS_CALL action, void *arg)
{
    __asm__ __volatile__( 
      "BKPT 0xAB	\n"	\
      "nop		\n" 	\
      "bx lr		\n"	\
        :::			\
    );
}

typedef struct paramopen{
    char *parampath;
    int   paramflags;
}paramopen_t;

typedef struct paramread{
    int   fd;
    void *buf;
    size_t *count;
}paramread_t;

typedef struct paramwrite{
    int   fd;
    void *buf;
    size_t count;
}paramwrite_t;

/*open*/
static int open(char *pathname, int flags){
    paramopen_t *parameteropen;

    parameteropen->parampath = pathname;
    parameteropen->paramflags    = 2;

    return simicall(SYS_OPEN, parameteropen);
}

/*
Value of mode
mode	0	1	2	3	4	5	6	7	8	9	10	11
	r	rb	r+	r+b	w	wb	w+	w+b	a	ab	a+	a+b
*/
/*read*/
static int read(int fd, void *buf, size_t count){
    paramread_t *parameterread;

    parameterread->fd = fd;
    parameterread->buf = buf;
    parameterread->count = count;

    return simicall(SYS_READ, parameterread);
}

/*close*/
static int close(int fd){
    return simicall(SYS_CLOSE, (void *)fd);
}

/*write*/
static int write(int fd, void *buf, size_t count){
    paramwrite_t *parameterwrite;

    parameterwrite->fd = fd;
    parameterwrite->buf = buf;
    parameterwrite->count = count;

    return simicall(SYS_WRITE, parameterwrite);
}

int main(void)
{
	/*ref rtenv*/	
	int fd;
	char c;

	fd = open("out", 0);

	read(fd, &c, 1);

	return 0;
}

