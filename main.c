#define STDIN  0
#define STDOUT 1
#define STDERR 2
#define ALIGN_MEM 512
#define SHA1_LEN 40

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct SHA1
{

}SHA1;

int main(void)
{
	char *buffer_in, *buffer_out;
	int len_in;

	buffer_in = (char*)malloc(ALIGN_MEM);
	buffer_out = (char*)malloc(SHA1_LEN);
	len_in = 0;

	printf("input : ");
	fflush(stdout);
	do
	{
		if(!(len_in % ALIGN_MEM) && len_in)
			buffer_in = (char*)realloc(buffer_in, len_in + ALIGN_MEM);
		read(STDIN, &buffer_in[len_in], 1);
	}while(buffer_in[len_in++] != '\n');
	buffer_in[--len_in] = '\x00';



	free(buffer_in);
	free(buffer_out);
}
