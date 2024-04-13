static void compose_response(char *type, char *host, char *buf, size_t len)
{
	char usn[256];
	char date[42];
	time_t now;

	/* RFC1123 date, as specified in RFC2616 */
	now = time(NULL);
	strftime(date, sizeof(date), "%a, %d %b %Y %T %Z", gmtime(&now));

	if (type) {
		if (!strcmp(type, uuid))
			type = NULL;
		else
			snprintf(usn, sizeof(usn), "%s::%s", uuid, type);
	}

	if (!type)
		strncpy(usn, uuid, sizeof(usn));

	snprintf(buf, len, "HTTP/1.1 200 OK\r\n"
		 "Server: %s\r\n"
		 "Date: %s\r\n"
		 "Location: http://%s:%d%s\r\n"
		 "ST: %s\r\n"
		 "EXT: \r\n"
		 "USN: %s\r\n"
		 "Cache-Control: max-age=%d\r\n"
		 "\r\n",
		 server_string,
		 date,
		 host, LOCATION_PORT, LOCATION_DESC,
		 type,
		 usn,
		 CACHE_TIMEOUT);
}
