static int read_l1_extended_headers(LHAFileHeader **header,
                                    LHAInputStream *stream)
{
	uint8_t *ext_header;
	size_t ext_header_len;

	for (;;) {

		if (!read_next_ext_header(header, stream,
		                          &ext_header, &ext_header_len)) {
			return 0;
		}


		if (ext_header_len == 0) {
			break;
		}


		if ((*header)->compressed_length < ext_header_len) {
			return 0;
		}

		(*header)->compressed_length -= ext_header_len;


		if (ext_header_len < 3) {
			return 0;
		}
	}

	return 1;
}
