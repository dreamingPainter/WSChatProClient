# include "int_put_into_CString.h"
# include "pch.h"
void bit32_data_into_buf(uint32_t src, char *dst_data)
{
	char buf_byte,*ptr;
	uint32_t src_buf;
	src = htons(src);
	ptr = dst_data;
	for (int i = 1; i < 4; i++)
	{
		src_buf = src;
		src_buf >> (32 - i * 8);
		*ptr = src_buf;
		ptr++;
	}
	dst_data = ptr;
	return;
}

void bit16_data_into_buf(uint16_t src, char *dst_data)
{
	char buf_byte,*ptr;
	ptr = dst_data;
	src = htons(src);
	buf_byte = HIBYTE(src);
	*ptr = buf_byte;
	ptr++;
	buf_byte = LOBYTE(src);
	*ptr = buf_byte;
	dst_data = ptr;
	return;
}