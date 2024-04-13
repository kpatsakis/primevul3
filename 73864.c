netscreen_seek_read(wtap *wth, gint64 seek_off,
	struct wtap_pkthdr *phdr, Buffer *buf,
	int *err, gchar **err_info)
{
	char		line[NETSCREEN_LINE_LENGTH];

	if (file_seek(wth->random_fh, seek_off, SEEK_SET, err) == -1) {
		return FALSE;
	}

	if (file_gets(line, NETSCREEN_LINE_LENGTH, wth->random_fh) == NULL) {
		*err = file_error(wth->random_fh, err_info);
		if (*err == 0) {
			*err = WTAP_ERR_SHORT_READ;
		}
		return FALSE;
	}

	return parse_netscreen_packet(wth->random_fh, phdr, buf, line,
	    err, err_info);
}
