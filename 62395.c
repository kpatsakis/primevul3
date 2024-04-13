static void process_level0_extended_area(LHAFileHeader *header,
                                         uint8_t *data, size_t data_len)
{

	if (!strncmp(header->compress_method, "-pm", 3)) {
		return;
	}


	switch (data[0]) {
		case LHA_OS_TYPE_UNIX:
		case LHA_OS_TYPE_OS9_68K:
			process_level0_unix_area(header, data, data_len);
			break;

		case LHA_OS_TYPE_OS9:
			process_level0_os9_area(header, data, data_len);
			break;

		default:
			break;
	}
}
