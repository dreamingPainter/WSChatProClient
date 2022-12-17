#include "crc_function.h"
#include "pch.h"
uint32_t crc32_function(unsigned char* data, unsigned short length,unsigned int poly)
{
	unsigned char i;
	int crc = 0xffffffff;  // Initial value
	while (length--)
	{
		crc ^= (int)(*data++) << 24;// crc ^=(uint32_t)(*data)<<24; data++;
		for (i = 0; i < 8; ++i)
		{
			if (crc & 0x80000000)
				crc = (crc << 1) ^ poly;
			else
				crc <<= 1;
		}
	}
	return crc;
}

/******************************************************************************
 * Name:    CRC-32/MPEG-2  x32+x26+x23+x22+x16+x12+x11+x10+x8+x7+x5+x4+x2+x+1
 * Poly:    0x4C11DB74C11DB7
 * Init:    0xFFFFFFFFFFFFFF
 * Refin:   False
 * Refout:  False
 * Xorout:  0x00000000000000
 * Note:
 *****************************************************************************/
unsigned long long crc64_function(FILE *fp,unsigned short length, unsigned long long poly)
{
	unsigned char i;   
	unsigned long long crc = 0xffffffffffffffff;  // Initial value
	unsigned char data;
	while (length--)
	{
		data = fgetc(fp);
		crc ^= (unsigned long long)(data) << 56;// crc ^=(uint32_t)(*data)<<24; data++;
		for (i = 0; i < 8; ++i)
		{
			if (crc & 0x8000000000000000)
				crc = (crc << 1) ^ poly;
			else
				crc <<= 1;
		}
	}
	return crc;
}


uint32_t calculate_chllenge_sum(char *ptr)//传入的ptr指向N
{
	unsigned char N;
	char *challenge_value_pos;
	uint32_t sum,bitset_byte_num;
	//value_32
	//uint16_t value_16;
	int value_32;
	int value_16;

	N = *ptr;
	sum = 0;
	bitset_byte_num = (N / 8) + ((N % 8) ? 1 : 0);
	challenge_value_pos = ptr + bitset_byte_num + 1;

	for (int count = 0; count < N; count++)
	{	//对于第i个数，其对应的置位在第几个字节的第几位
		unsigned char byte_buf = *(ptr + 1 + count / 8);
		if (byte_buf&(0x80>>(7-count%8)))//4字节
		{
			value_32 = *(uint32_t*)challenge_value_pos;
			sum += ntohl(value_32);
			challenge_value_pos += 4;
		}
		else{//2字节
			value_16 = *(uint16_t*)challenge_value_pos;
			sum += ntohs(value_16);
			challenge_value_pos += 2;
		}
	}
	return sum;
}