# include "int_put_into_CString.h"
# include "pch.h"
void bit32_data_into_buf(uint32_t src, CStringA* dst_data)
{
	char buf_byte;
	uint32_t src_buf;
	src = htons(src);
	for (int i = 1; i < 4; i++)
	{
		src_buf = src;
		src_buf >> (32 - i * 8);
		buf_byte = src_buf;
		dst_data = dst_data + buf_byte;
	}
	return;
}

void bit16_data_into_buf(uint16_t src, CStringA* dst_data)
{
	char buf_byte;
	src = htons(src);
	buf_byte = HIBYTE(src);
	dst_data = dst_data + buf_byte;
	buf_byte = LOBYTE(src);
	dst_data = dst_data + buf_byte;
}