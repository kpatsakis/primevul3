static void send_search(struct ifsock *ifs, char *type)
{
	ssize_t num;
	char buf[MAX_PKT_SIZE];
	struct sockaddr dest;

	memset(buf, 0, sizeof(buf));
	compose_search(type, buf, sizeof(buf));
	compose_addr((struct sockaddr_in *)&dest, MC_SSDP_GROUP, MC_SSDP_PORT);

	logit(LOG_DEBUG, "Sending M-SEARCH ...");
	num = sendto(ifs->out, buf, strlen(buf), 0, &dest, sizeof(struct sockaddr_in));
	if (num < 0)
		logit(LOG_WARNING, "Failed sending SSDP M-SEARCH");
}
