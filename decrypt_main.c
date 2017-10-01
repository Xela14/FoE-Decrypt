#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


size_t parse_footer(uint8_t *buff, size_t file_len)
{
	return (  (buff[file_len - 1] << 24) | (buff[file_len - 2] << 16) | (buff[file_len - 3] << 8) | (buff[file_len - 4])  );
}

int main()
{	

	FILE *main_swf = fopen("Main.swf", "rb");
	fseek(main_swf, 0, SEEK_END);
	size_t file_len = ftell(main_swf);
	uint8_t *buff = malloc((file_len) * sizeof(*buff));
	rewind(main_swf);
	fread(buff, file_len, 1, main_swf);
	fclose(main_swf);

	
	size_t   real_len = parse_footer(buff, file_len);
	// Great use of a lambda for code readability
	uint32_t xor_key  = ({ uint32_t x(size_t l){ while (l > 255)	l >>= 1; return l;} x(real_len);});

	for (int i = 0; i < real_len; ++i)
	{
		buff[i] ^= xor_key;
	}
	*((uint32_t *)(buff + (file_len - 5))) = 0;			 // clearly the best way to zero out the last 4 bytes of the array

	FILE *out = fopen("foe.swf","wb");
	fwrite(buff, real_len, 1, out);
	fclose(out);
}

