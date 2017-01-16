#define STDIN  0
#define STDOUT 1
#define STDERR 2
#define ALIGN_MEM 512
#define SHA1_LEN 40

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *buffer_in, *buffer_out;
unsigned char Block[64] = {}, Result[20] = {};
const unsigned int K[4] = {0x5A827999, 0x6ED9EBA1, 0x8F1BBCDC, 0xCA62C1D6};
unsigned int H[5] = {0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0};
unsigned int W[80] = {}, A = 0, B = 0, C = 0, D = 0, E = 0, t = 0, temp = 0, len_high = 0, len_low = 0;
unsigned long long int len_in = 0, index_in = 0, index_block = 0, i = 0;

unsigned int ROL(int bits, unsigned int dword)
{
	return ((dword << bits) & 0xFFFFFFFF) | ((dword & 0xFFFFFFFF) >> (32-bits));
}

void Update()
{
	for(t = 0; t < 16; t++)
	{
		W[t]  = ((unsigned int)Block[t * 4 + 0]) << 24;
		W[t] |= ((unsigned int)Block[t * 4 + 1]) << 16;
		W[t] |= ((unsigned int)Block[t * 4 + 2]) << 8;
		W[t] |= ((unsigned int)Block[t * 4 + 3]);
	}
	
	for(t = 16; t < 80; t++)
		W[t] = ROL(1, W[t-3] ^ W[t-8] ^ W[t-14] ^ W[t-16]);
				
	A = H[0];
	B = H[1];
	C = H[2];
	D = H[3];
	E = H[4];

	for(t = 0; t < 20; t++)
	{
		temp = ROL(5, A) + ((B & C) | ((~B) & D)) + E + W[t] + K[0];
		E = D;
		D = C;
		C = ROL(30, B);
		B = A;
		A = temp;
	}

	for(t = 20; t < 40; t++)
	{
		temp = ROL(5, A) + (B ^ C ^ D) + E + W[t] + K[1];
		E = D;
		D = C;
		C = ROL(30, B);
		B = A;
		A = temp;
	}

	for(t = 40; t < 60; t++)
	{
		temp = ROL(5, A) + ((B & C) | (B & D) | (C & D)) + E + W[t] + K[2];
		E = D;
		D = C;
		C = ROL(30, B);
		B = A;
		A = temp;
	}


	for(t = 60; t < 80; t++)
	{
		temp = ROL(5, A) + (B ^ C ^ D) + E + W[t] + K[3];
		E = D;
		D = C;
		C = ROL(30, B);
		B = A;
		A = temp;
	}

	H[0] += A;
	H[1] += B;
	H[2] += C;
	H[3] += D;
	H[4] += E;

	index_block = 0;
}

int main(void)
{
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
		len_low += 8;
		if (len_low == 0)
			len_high++;
		if (index_block == 64)
			Update();
	}

	if (index_block > 55)
	{
		Block[index_block++] = 0x80;
		while(index_block < 64)
			Block[index_block++] = 0;
		Update();
		while(index_block < 56)
			Block[index_block++] = 0;
	}
	else
	{
		Block[index_block++] = 0x80;
		while(index_block < 56)
			Block[index_block++] = 0;
	}

	Block[56] = len_high >> 24;
    Block[57] = len_high >> 16;
    Block[58] = len_high >> 8;
    Block[59] = len_high;
    Block[60] = len_low >> 24;
    Block[61] = len_low >> 16;
    Block[62] = len_low >> 8;
    Block[63] = len_low;
	Update();

	for(i = 0; i < 20; i++)
		Result[i] = H[i>>2] >> 8 * ( 3 - ( i & 0x03 ) );

	for(i = 0; i < 20; i++)
		printf("%02x", Result[i]);
	putchar('\n');
	free(buffer_in);
	free(buffer_out);
}
