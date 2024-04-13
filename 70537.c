static void lsb_init(void)
{
	FILE *fp;
	char *ptr;
	char line[80];
	const char *file = "/etc/lsb-release";

	fp = fopen(file, "r");
	if (!fp) {
	fallback:
		logit(LOG_WARNING, "No %s found on system, using built-in server string.", file);
		return;
	}

	while (fgets(line, sizeof(line), fp)) {
		line[strlen(line) - 1] = 0;

		ptr = strstr(line, "DISTRIB_ID");
		if (ptr && (ptr = strchr(ptr, '=')))
			os = strdup(++ptr);

		ptr = strstr(line, "DISTRIB_RELEASE");
		if (ptr && (ptr = strchr(ptr, '=')))
			ver = strdup(++ptr);
	}
	fclose(fp);

	if (os && ver)
		snprintf(server_string, sizeof(server_string), "%s/%s UPnP/1.0 %s/%s",
			 os, ver, PACKAGE_NAME, PACKAGE_VERSION);
	else
		goto fallback;

	logit(LOG_DEBUG, "Server: %s", server_string);
}
