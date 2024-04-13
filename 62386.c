static int decode_level1_header(LHAFileHeader **header, LHAInputStream *stream)
{
	unsigned int ext_header_start;

	if (!decode_level0_header(header, stream)) {
		return 0;
	}


	ext_header_start = RAW_DATA_LEN(header) - 2;

	if (!read_l1_extended_headers(header, stream)
	 || !decode_extended_headers(header, ext_header_start)) {
		return 0;
	}

	return 1;
}
