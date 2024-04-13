int wmain(int argc, wchar_t** wargv)
{
	int i;
	int res;
	size_t len;
	size_t res_len;
	char **argv;
	argv = (char **)malloc(argc*sizeof(wchar_t *));
	for (i = 0; i < argc; i++) {
		wchar_t *src_str = wargv[i];
		len = UTF8_MAX_BYTES_PER_CHAR * gf_utf8_wcslen(wargv[i]);
		argv[i] = (char *)malloc(len + 1);
		res_len = gf_utf8_wcstombs(argv[i], len, &src_str);
		argv[i][res_len] = 0;
		if (res_len > len) {
			fprintf(stderr, "Length allocated for conversion of wide char to UTF-8 not sufficient\n");
			return -1;
		}
	}
	res = mp4client_main(argc, argv);
	for (i = 0; i < argc; i++) {
		free(argv[i]);
	}
	free(argv);
	return res;
}
