string_num_dots(const char *s) {
	int count = 0;
	while ((s = strchr(s, '.'))) {
		s++;
		count++;
	}
	return count;
}
