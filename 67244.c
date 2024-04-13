static int bin_pe_read_metadata_string(char* to, char* from) {
	int covered = 0;
	while (covered < MAX_METADATA_STRING_LENGTH) {
		to[covered] = from[covered];
		if (from[covered] == '\0') {
			covered += 1;
			break;
		}
		covered++;
	}
	while (covered % 4 != 0) { covered++; }
	return covered;
}
