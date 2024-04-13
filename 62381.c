static int check_l0_checksum(uint8_t *header, size_t header_len, size_t csum)
{
	unsigned int result;
	unsigned int i;

	result = 0;

	for (i = 0; i < header_len; ++i) {
		result += header[i];
	}

	return (result & 0xff) == csum;
}
