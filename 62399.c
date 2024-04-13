static int split_header_filename(LHAFileHeader *header)
{
	char *sep;
	char *new_filename;


	sep = strrchr(header->filename, '/');

	if (sep != NULL) {
		new_filename = strdup(sep + 1);

		if (new_filename == NULL) {
			return 0;
		}

		*(sep + 1) = '\0';
		header->path = header->filename;
		header->filename = new_filename;
	}

	return 1;
}
