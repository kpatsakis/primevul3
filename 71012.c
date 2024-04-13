lease_file_remove(unsigned short eport, int proto)
{
	FILE* fd, *fdt;
	int tmp;
	char buf[512];
	char str[32];
	char tmpfilename[128];
	int str_size, buf_size;


	if (lease_file == NULL) return 0;

	if (strlen( lease_file) + 7 > sizeof(tmpfilename)) {
		syslog(LOG_ERR, "Lease filename is too long");
		return -1;
	}

	snprintf( tmpfilename, sizeof(tmpfilename), "%sXXXXXX", lease_file);

	fd = fopen( lease_file, "r");
	if (fd==NULL) {
		return 0;
	}

	snprintf( str, sizeof(str), "%s:%u", proto_itoa(proto), eport);
	str_size = strlen(str);

	tmp = mkstemp(tmpfilename);
	if (tmp==-1) {
		fclose(fd);
		syslog(LOG_ERR, "could not open temporary lease file");
		return -1;
	}
	fchmod(tmp, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	fdt = fdopen(tmp, "a");

	buf[sizeof(buf)-1] = 0;
	while( fgets(buf, sizeof(buf)-1, fd) != NULL) {
		buf_size = strlen(buf);

		if (buf_size < str_size || strncmp(str, buf, str_size)!=0) {
			fwrite(buf, buf_size, 1, fdt);
		}
	}
	fclose(fdt);
	fclose(fd);

	if (rename(tmpfilename, lease_file) < 0) {
		syslog(LOG_ERR, "could not rename temporary lease file to %s", lease_file);
		remove(tmpfilename);
	}

	return 0;

}
