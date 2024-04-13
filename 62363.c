removeDevice(const struct header * headers)
{
	struct device ** pp = &devlist;
	struct device * p = *pp;	/* = devlist */
	while(p)
	{
		if(  p->headers[HEADER_NT].l == headers[HEADER_NT].l
		  && (0==memcmp(p->headers[HEADER_NT].p, headers[HEADER_NT].p, headers[HEADER_NT].l))
		  && p->headers[HEADER_USN].l == headers[HEADER_USN].l
		  && (0==memcmp(p->headers[HEADER_USN].p, headers[HEADER_USN].p, headers[HEADER_USN].l)) )
		{
			syslog(LOG_INFO, "remove device : %.*s", headers[HEADER_USN].l, headers[HEADER_USN].p);
			*pp = p->next;
			free(p);
			return -1;
		}
		pp = &p->next;
		p = *pp;	/* p = p->next; */
	}
	syslog(LOG_WARNING, "device not found for removing : %.*s", headers[HEADER_USN].l, headers[HEADER_USN].p);
	return 0;
}
