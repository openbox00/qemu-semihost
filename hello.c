#include <stdio.h>
#include <inttypes.h>

#define size 100

int main(void)
{
	FILE *fileread,*filewrite;
	unsigned char pathread[size];
	unsigned char buffer[size];	
	unsigned char writebuffer[size];	

	//open and read(open host file, and read file then print out)	
	printf("You can modify the file in Host\n");
	printf("Please enter the file path in Host(no more than 100 char):\n");
	scanf("%100s", pathread);
	/*that scanf can work */
	fflush(stdin);
	fileread = fopen(pathread, "r");
	if (fileread == NULL){
	printf("Please enter the right path!!\n");
	}else{
	fread(buffer, sizeof(buffer),1,fileread);
	printf("%s content is %s\n",pathread,buffer);
	}
	fclose(fileread);

	//write (write back to host file)	
	filewrite = fopen(pathread, "a");
	printf("\nPlease enter the content you want to write in file(no more than 100 char):\n");	
	/*add limit for scanf*/
	scanf("%100[^\n]",writebuffer);

	/*that scanf can work */
	fflush(stdin);
	fprintf(filewrite, "%s", &writebuffer);
   	fclose(filewrite);	
	return 0;
}
