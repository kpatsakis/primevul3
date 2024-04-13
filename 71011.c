lease_file_add(unsigned short eport,
               const char * iaddr,
               unsigned short iport,
               int proto,
               const char * desc,
               unsigned int timestamp)
{
	FILE * fd;

	if (lease_file == NULL) return 0;

	fd = fopen( lease_file, "a");
	if (fd==NULL) {
		syslog(LOG_ERR, "could not open lease file: %s", lease_file);
		return -1;
	}

	/* convert our time to unix time
     * if LEASEFILE_USE_REMAINING_TIME is defined, only the remaining time is stored */
	if (timestamp != 0) {
		timestamp -= upnp_time();
#ifndef LEASEFILE_USE_REMAINING_TIME
		timestamp += time(NULL);
#endif
	}

	fprintf(fd, "%s:%hu:%s:%hu:%u:%s\n",
	        proto_itoa(proto), eport, iaddr, iport,
	        timestamp, desc);
	fclose(fd);

	return 0;
}
