static int decode_extended_headers(LHAFileHeader **header,
                                   unsigned int offset)
{
	unsigned int field_size;
	uint8_t *ext_header;
	size_t ext_header_len;
	size_t available_length;


	if ((*header)->header_level == 3) {
		field_size = 4;
	} else {
		field_size = 2;
	}

	available_length = RAW_DATA_LEN(header) - offset - field_size;

	while (offset <= RAW_DATA_LEN(header) - field_size) {
		ext_header = &RAW_DATA(header, offset + field_size);

		if (field_size == 4) {
			ext_header_len
			    = lha_decode_uint32(&RAW_DATA(header, offset));
		} else {
			ext_header_len
			    = lha_decode_uint16(&RAW_DATA(header, offset));
		}


		if (ext_header_len == 0) {
			break;
		} else if (ext_header_len < field_size + 1
		        || ext_header_len > available_length) {
			return 0;
		}


		lha_ext_header_decode(*header, ext_header[0], ext_header + 1,
		                      ext_header_len - field_size - 1);


		offset += ext_header_len;
		available_length -= ext_header_len;
	}

	return 1;
}
