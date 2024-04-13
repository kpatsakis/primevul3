static int filter_iface(char *ifname, char *iflist[], size_t num)
{
	size_t i;

	if (!num) {
		logit(LOG_DEBUG, "No interfaces to filter, using all with an IP address.");
		return 0;
	}

	logit(LOG_DEBUG, "Filter %s?  Comparing %zd entries ...", ifname, num);
	for (i = 0; i < num; i++) {
		logit(LOG_DEBUG, "Filter %s?  Comparing with %s ...", ifname, iflist[i]);
		if (!strcmp(ifname, iflist[i]))
			return 0;
	}

	return 1;
}
