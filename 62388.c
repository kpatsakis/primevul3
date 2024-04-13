static void fix_msdos_allcaps(LHAFileHeader *header)
{
	unsigned int i;
	int is_allcaps;


	is_allcaps = 1;

	if (header->path != NULL) {
		for (i = 0; header->path[i] != '\0'; ++i) {
			if (islower((unsigned) header->path[i])) {
				is_allcaps = 0;
				break;
			}
		}
	}

	if (is_allcaps && header->filename != NULL) {
		for (i = 0; header->filename[i] != '\0'; ++i) {
			if (islower((unsigned) header->filename[i])) {
				is_allcaps = 0;
				break;
			}
		}
	}


	if (is_allcaps) {
		if (header->path != NULL) {
			for (i = 0; header->path[i] != '\0'; ++i) {
				header->path[i]
				    = tolower((unsigned) header->path[i]);
			}
		}
		if (header->filename != NULL) {
			for (i = 0; header->filename[i] != '\0'; ++i) {
				header->filename[i]
				    = tolower((unsigned) header->filename[i]);
			}
		}
	}
}
