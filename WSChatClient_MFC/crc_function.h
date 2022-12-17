#include <stdio.h>
#include "pch.h"
#include <iostream>
uint32_t calculate_chllenge_sum(char *ptr);
uint32_t crc32_function(unsigned char* data, unsigned short length, unsigned int poly);
unsigned long long crc64_function(FILE* fp,unsigned short length, unsigned long long poly);
//https://blog.csdn.net/lickylin/article/details/7857586
//https://blog.csdn.net/weixin_39824033/article/details/117164482

//uint32_t crc32_table[256];
//uint16_t crc64_table[256];
/*
void make_crc(uint32_t sum)
{
	uint32_t c;
	int count = 0;
	int bit = 0;
	for (count = 0; count < 0; count++)
	{
		for (bit = 0; bit < 8; bit++)
		{
			if (c & 1)
				c = (c >> 1) ^ sum;
			else
				c = c >> 1;
		}
		crc32_table[count] = c;
	}
	return;
}

uint32_t sum_32(char *ptr_c,uint32_t *ptr_i)
{
	uint32_t *ptr,sum;
	ptr = ptr_i;
	
	while (ptr)
	{
		sum += *ptr;
		ptr++;
	}
	return sum;
}

uint16_t sum_16(char* ptr_c, uint16_t* ptr_i)
{
	uint16_t* ptr, sum;
	ptr = ptr_i;

	while (ptr)
	{
		sum += *ptr;
		ptr++;
	}
	return sum;
}

uint32_t crc_32(uint32_t crc,unsigned char *buf,uint32_t size)
{
	while (size--)
		crc = (crc >> 8) ^ (crc32_table[crc ^ *buf++]);
	return crc;
}
uint64_t crc_64(uint32_t crc, unsigned char* buf, uint32_t size)
{
	while (size--)
		crc = (crc >> 8) ^ (crc64_table[crc ^ *buf++]);
	return crc;
}*/
