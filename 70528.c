static void compose_notify(char *type, char *host, char *buf, size_t len)
{
	char usn[256];

	if (type) {
		if (!strcmp(type, SSDP_ST_ALL))
			type = NULL;
		else
			snprintf(usn, sizeof(usn), "%s::%s", uuid, type);
	}

	if (!type) {
		type = usn;
		strncpy(usn, uuid, sizeof(usn));
	}

	snprintf(buf, len, "NOTIFY * HTTP/1.1\r\n"
		 "Host: %s:%d\r\n"
		 "Server: %s\r\n"
		 "Location: http://%s:%d%s\r\n"
		 "NT: %s\r\n"
		 "NTS: ssdp:alive\r\n"
		 "USN: %s\r\n"
		 "Cache-Control: max-age=%d\r\n"
		 "\r\n",
		 MC_SSDP_GROUP, MC_SSDP_PORT,
		 server_string,
		 host, LOCATION_PORT, LOCATION_DESC,
		 type,
		 usn,
		 CACHE_TIMEOUT);
}
