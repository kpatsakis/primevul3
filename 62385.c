static int decode_level0_header(LHAFileHeader **header, LHAInputStream *stream)
{
	uint8_t header_len;
	uint8_t header_csum;
	size_t path_len;
	size_t min_len;

	header_len = RAW_DATA(header, 0);
	header_csum = RAW_DATA(header, 1);


	switch ((*header)->header_level) {
		case 0:
			min_len = LEVEL_0_MIN_HEADER_LEN;
			break;
		case 1:
			min_len = LEVEL_1_MIN_HEADER_LEN;
			break;

		default:
			return 0;
	}

	if (header_len < min_len) {
		return 0;
	}


	if (!extend_raw_data(header, stream,
	                     header_len + 2 - RAW_DATA_LEN(header))) {
		return 0;
	}


	if (!check_l0_checksum(&RAW_DATA(header, 2),
	                       RAW_DATA_LEN(header) - 2,
	                       header_csum)) {
		return 0;
	}


	memcpy((*header)->compress_method, &RAW_DATA(header, 2), 5);
	(*header)->compress_method[5] = '\0';


	(*header)->compressed_length = lha_decode_uint32(&RAW_DATA(header, 7));
	(*header)->length = lha_decode_uint32(&RAW_DATA(header, 11));


	(*header)->timestamp = decode_ftime(&RAW_DATA(header, 15));


	path_len = RAW_DATA(header, 21);

	if (min_len + path_len > header_len) {
		return 0;
	}


	if ((*header)->header_level == 0) {
		(*header)->os_type = LHA_OS_TYPE_UNKNOWN;
	} else {
		(*header)->os_type = RAW_DATA(header, 24 + path_len);
	}


	if (!process_level0_path(*header, &RAW_DATA(header, 22), path_len)) {
		return 0;
	}


	(*header)->crc = lha_decode_uint16(&RAW_DATA(header, 22 + path_len));


	if ((*header)->header_level == 0
	 && header_len > LEVEL_0_MIN_HEADER_LEN + path_len) {
		process_level0_extended_area(*header,
		  &RAW_DATA(header, LEVEL_0_MIN_HEADER_LEN + 2 + path_len),
		  header_len - LEVEL_0_MIN_HEADER_LEN - path_len);
	}

	return 1;
}
