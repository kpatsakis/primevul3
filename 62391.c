char *lha_file_header_full_path(LHAFileHeader *header)
{
	char *path;
	char *filename;
	char *result;

	if (header->path != NULL) {
		path = header->path;
	} else {
		path = "";
	}

	if (header->filename != NULL) {
		filename = header->filename;
	} else {
		filename = "";
	}

	result = malloc(strlen(path) + strlen(filename) + 1);

	if (result == NULL) {
		return NULL;
	}

	sprintf(result, "%s%s", path, filename);

	return result;
}
