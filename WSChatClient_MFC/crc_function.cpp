#include "crc_function.h"
#include "pch.h"
uint32_t crc32_function(unsigned char* data, unsigned short length, int poly)
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
uint32_t calculate_chllenge_sum(char *ptr)//传入的ptr指向N
{
	unsigned char N;
	char *challenge_value_pos;
	uint32_t sum,bitset_byte_num,value_32;
	uint16_t value_16;

	N = *ptr;
	bitset_byte_num = (N / 8) + ((N % 8) ? 1 : 0);
	challenge_value_pos = ptr + bitset_byte_num + 1;

	for (int count = 0; count < N; count++)
	{	//对于第i个数，其对应的置位在第几个字节的第几位
		unsigned char byte_buf = *(ptr + count / 8 + ((count % 8) ? 1 : 0));
		if (byte_buf&(0x01<<(count%8)))//4字节
		{
			value_32 = *(uint32_t*)challenge_value_pos;
			sum += ntohl(value_32);
			challenge_value_pos += 4;
		}
		else {//2字节
			value_16 = *(uint16_t*)challenge_value_pos;
			sum += ntohl(value_16);
			challenge_value_pos += 2;
		}
	}
	return sum;
}