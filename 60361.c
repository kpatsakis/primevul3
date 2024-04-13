config_nameserver_from_reg_key(struct evdns_base *base, HKEY key, const TCHAR *subkey)
{
	char *buf;
	DWORD bufsz = 0, type = 0;
	int status = 0;

	ASSERT_LOCKED(base);
	if (RegQueryValueEx(key, subkey, 0, &type, NULL, &bufsz)
	    != ERROR_MORE_DATA)
		return -1;
	if (!(buf = mm_malloc(bufsz)))
		return -1;

	if (RegQueryValueEx(key, subkey, 0, &type, (LPBYTE)buf, &bufsz)
	    == ERROR_SUCCESS && bufsz > 1) {
		status = evdns_nameserver_ip_add_line(base,buf);
	}

	mm_free(buf);
	return status;
}
