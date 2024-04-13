upnpevents_addSubscriber(const char * eventurl,
                         const char * callback, int callbacklen,
                         int timeout)
{
	struct subscriber * tmp;
	/*static char uuid[42];*/
	/* "uuid:00000000-0000-0000-0000-000000000000"; 5+36+1=42bytes */
	syslog(LOG_DEBUG, "addSubscriber(%s, %.*s, %d)",
	       eventurl, callbacklen, callback, timeout);
	/*strncpy(uuid, uuidvalue, sizeof(uuid));
	uuid[sizeof(uuid)-1] = '\0';*/
	tmp = newSubscriber(eventurl, callback, callbacklen);
	if(!tmp)
		return NULL;
	if(timeout)
		tmp->timeout = upnp_time() + timeout;
	LIST_INSERT_HEAD(&subscriberlist, tmp, entries);
	upnp_event_create_notify(tmp);
	return tmp->uuid;
}
