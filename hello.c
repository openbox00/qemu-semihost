#include <stdio.h>
#include <stdlib.h>
//#include <inttypes.h>
//#include <stddef.h>


#define S_IFIFO 1
#define S_IMSGQ 2
#define O_CREAT 4

#define HELLO_WORLD "Hello World\n"

typedef struct param{
    int   pdInt;
    void *pdPtr;
    char *pdChrPtr;
} param_t;


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
	SYS_READC	= 0x07,
	angel_SWIreason_EnterSVC = 0x17,
	angel_SWIreason_ReportException	= 0x18
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


/*open*/
static int open(char *pathname, int flags){
    param_t parameter[3] = {0};

    parameter[0].pdChrPtr = pathname;
    parameter[1].pdInt    = 2;

    return simicall(SYS_OPEN, parameter);
}

/*
Value of mode
mode	0	1	2	3	4	5	6	7	8	9	10	11
	r	rb	r+	r+b	w	wb	w+	w+b	a	ab	a+	a+b
*/

/*read*/
static int read(int fd, void *ptr, int numbytes){
    param_t parameter[3] = {0};

    parameter[0].pdInt = fd;
    parameter[1].pdPtr = ptr;
    parameter[2].pdInt = numbytes;

    return simicall(SYS_READ, parameter);
}


/*close*/
static int close(int fd){
    return simicall(SYS_CLOSE, (void *)fd);
}

/*write*/
static int write(int fd, void *ptr, int numbytes){
    param_t parameter[3] = {0};

    parameter[0].pdInt = fd;
    parameter[1].pdPtr = ptr;
    parameter[2].pdInt = numbytes;

    return simicall(SYS_WRITE, parameter);
}

/*
void print(char *str){
	write(open("x.txt", 2), str,strlen(str)+1);
}

void scanf(char *x)
{
	read(open("x.txt", 0), x, 1);
}
*/
int main(void)
{
	int fd;
	char ch;
	/*ref rtenv*/
	fd = open("x.txt",2);

	/*flag=2, means can read and write*/
	write(fd, HELLO_WORLD, sizeof(HELLO_WORLD));
	

//	fd = open("x.txt", 2);


//	read(fdin, &ch, 1);


//	close(fd);


#if 0
	unsigned char path[size];
	unsigned char buffer[size];	
	unsigned char writebuffer[size];	

	//open and read(open host file, and read file then print out)	
	printf("You can modify the file in Host\n");
	printf("Please enter the file path in Host :\n");
	scanf("%s", path);
	/*let scanf can work */
	fflush(stdin);

	if (fd == NULL){
	printf("Please enter the right path!!\n");
	}else{
	read(buffer, sizeof(buffer),1,fd);
	printf("%s content is %s\n",path,buffer);
	//write (write back to host file)	
	printf("\nPlease enter the content you want to write in file :\n");	
	scanf("%[^\n]",writebuffer);
	/*let scanf can work */
	fflush(stdin);
	write(fd, "%s", &writebuffer);
	}
	close(fd);
#endif
	return 0;
}
