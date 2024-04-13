static int usage(int code)
{
	printf("Usage: %s [-dhv] [-i SEC] [IFACE [IFACE ...]]\n"
	       "\n"
	       "    -d        Developer debug mode\n"
	       "    -h        This help text\n"
	       "    -i SEC    SSDP notify interval (30-900), default %d sec\n"
	       "    -r SEC    Interface refresh interval (5-1800), default %d sec\n"
	       "    -v        Show program version\n"
	       "\n"
	       "Bug report address: %-40s\n", PACKAGE_NAME, NOTIFY_INTERVAL, REFRESH_INTERVAL, PACKAGE_BUGREPORT);

	return code;
}
