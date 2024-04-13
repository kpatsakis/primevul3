static void compose_search(char *type, char *buf, size_t len)
{
	snprintf(buf, len, "M-SEARCH * HTTP/1.1\r\n"
		 "Host: %s:%d\r\n"
		 "MAN: \"ssdp:discover\"\r\n"
		 "MX: 1\r\n"
		 "ST: %s\r\n"
		 "User-Agent: %s\r\n"
		 "\r\n",
		 MC_SSDP_GROUP, MC_SSDP_PORT,
		 type,
		 server_string);
}
