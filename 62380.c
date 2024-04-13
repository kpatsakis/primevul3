static int check_common_crc(LHAFileHeader *header)
{
	uint16_t crc;

	crc = 0;
	lha_crc16_buf(&crc, header->raw_data, header->raw_data_len);

	return crc == header->common_crc;
}
