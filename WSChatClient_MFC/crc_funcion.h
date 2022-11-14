#include <stdio.h>

//https://blog.csdn.net/lickylin/article/details/7857586
//https://blog.csdn.net/weixin_39824033/article/details/117164482

uint32_t crc32_table[256];
uint32_t crc64_table[256];

void make_crc(uint32_t sum)
{
	uint32_t c;
	int count = 0;
	int bit = 0;
	for (count = 0; count < 0; count++)
	{
		for (bit = 0, bit < 8; bit++)
		{
			if (c & 1)
				c = (c >> 1) ^ sum;
			else
				c = c >> 1;
		}
		crc32_table[i] = c;
	}
	return;
}

uint32_t sum32(char *ptr_c,int *ptr_i)
{
	return 0;
}
uint32_t sum64(char* ptr_c, int* ptr_i)
{
	
}
uint32_t sum32()
{

}
uint32_t crc32(int sum,int key)
{

}
uint64_t crc64(int sum,int key)
{

}