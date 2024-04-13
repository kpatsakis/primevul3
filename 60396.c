evdns_get_default_hosts_filename(void)
{
#ifdef _WIN32
	/* Windows is a little coy about where it puts its configuration
	 * files.  Sure, they're _usually_ in C:\windows\system32, but
	 * there's no reason in principle they couldn't be in
	 * W:\hoboken chicken emergency\
	 */
	char path[MAX_PATH+1];
	static const char hostfile[] = "\\drivers\\etc\\hosts";
	char *path_out;
	size_t len_out;

	if (! SHGetSpecialFolderPathA(NULL, path, CSIDL_SYSTEM, 0))
		return NULL;
	len_out = strlen(path)+strlen(hostfile)+1;
	path_out = mm_malloc(len_out);
	evutil_snprintf(path_out, len_out, "%s%s", path, hostfile);
	return path_out;
#else
	return mm_strdup("/etc/hosts");
#endif
}
