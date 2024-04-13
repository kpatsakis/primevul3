static int process_level0_path(LHAFileHeader *header, uint8_t *data,
                               size_t data_len)
{
	unsigned int i;


	if (data_len == 0) {
		return 1;
	}

	header->filename = malloc(data_len + 1);

	if (header->filename == NULL) {
		return 0;
	}

	memcpy(header->filename, data, data_len);
	header->filename[data_len] = '\0';


	for (i = 0; i < data_len; ++i) {
		if (header->filename[i] == '\\') {
			header->filename[i] = '/';
		}
	}

	return split_header_filename(header);
}
