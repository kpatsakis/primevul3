static const char * read_line_input(char * line, int maxSize, Bool showContent) {
	char read;
	int i = 0;
	if (fflush( stderr ))
		perror("Failed to flush buffer %s");
	do {
		line[i] = '\0';
		if (i >= maxSize - 1)
			return line;
		read = getch();
		if (read == 8 || read == 127) {
			if (i > 0) {
				fprintf(stderr, "\b \b");
				i--;
			}
		} else if (read > 32) {
			fputc(showContent ? read : '*', stderr);
			line[i++] = read;
		}
		fflush(stderr);
	} while (read != '\n');
	if (!read)
		return 0;
	return line;
}
