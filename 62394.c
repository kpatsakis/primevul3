static int parse_symlink(LHAFileHeader *header)
{
	char *fullpath;
	char *p;


	fullpath = lha_file_header_full_path(header);

	if (fullpath == NULL) {
		return 0;
	}

	p = strchr(fullpath, '|');

	if (p == NULL) {
		free(fullpath);
		return 0;
	}

	header->symlink_target = strdup(p + 1);

	if (header->symlink_target == NULL) {
		free(fullpath);
		return 0;
	}


	*p = '\0';

	free(header->path);
	free(header->filename);
	header->path = NULL;
	header->filename = fullpath;


	return split_header_filename(header);
}
