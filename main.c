#define STDIN  0
#define STDOUT 1
#define STDERR 2
#define ALIGN_MEM 512
#define SHA1_LEN 40

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

unsigned int ROL(int bits, unsigned dword)
{
	return ((dword << bits) & 0xFFFFFFFF) | ((dword & 0xFFFFFFFF) >> (32-bits));
}

int main(void)
{
	char *buffer_in, *buffer_out;
	unsigned char Block[64] = {};
	const unsigned int K[4] = {0x5A827999, 0x6ED9EBA1, 0x8F1BBCDC, 0xCA62C1D6};
	unsigned int H[5] = {0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0};
	unsigned int W[80] = {}, A = 0, B = 0, C = 0, D = 0, E = 0;
	unsigned long long int len_in = 0, index_in = 0, index_block = 0;

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

	while(len_in--)
	{
		Block[index_block++] = buffer_in[index_in++];
		if (index_block == 64)
		{
			
			index_block = 0;
		}

	}

	free(buffer_in);
	free(buffer_out);
}
