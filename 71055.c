upnp_event_create_notify(struct subscriber * sub)
{
	struct upnp_event_notify * obj;
	/*struct timeval sock_timeout;*/

	obj = calloc(1, sizeof(struct upnp_event_notify));
	if(!obj) {
		syslog(LOG_ERR, "%s: calloc(): %m", "upnp_event_create_notify");
		return;
	}
	obj->sub = sub;
	obj->state = ECreated;
#ifdef ENABLE_IPV6
	obj->s = socket((obj->sub->callback[7] == '[') ? PF_INET6 : PF_INET,
	                SOCK_STREAM, 0);
#else
	obj->s = socket(PF_INET, SOCK_STREAM, 0);
#endif
	if(obj->s<0) {
		syslog(LOG_ERR, "%s: socket(): %m", "upnp_event_create_notify");
		goto error;
	}
#if 0 /* does not work for non blocking connect() */
	/* set timeout to 3 seconds */
	sock_timeout.tv_sec = 3;
	sock_timeout.tv_usec = 0;
	if(setsockopt(obj->s, SOL_SOCKET, SO_RCVTIMEO, &sock_timeout, sizeof(struct timeval)) < 0) {
		syslog(LOG_WARNING, "%s: setsockopt(SO_RCVTIMEO): %m",
		       "upnp_event_create_notify");
	}
	sock_timeout.tv_sec = 3;
	sock_timeout.tv_usec = 0;
	if(setsockopt(obj->s, SOL_SOCKET, SO_SNDTIMEO, &sock_timeout, sizeof(struct timeval)) < 0) {
		syslog(LOG_WARNING, "%s: setsockopt(SO_SNDTIMEO): %m",
		       "upnp_event_create_notify");
	}
#endif
	/* set socket non blocking */
	if(!set_non_blocking(obj->s)) {
		syslog(LOG_ERR, "%s: set_non_blocking(): %m",
		       "upnp_event_create_notify");
		goto error;
	}
	if(sub)
		sub->notify = obj;
	LIST_INSERT_HEAD(&notifylist, obj, entries);
	return;
error:
	if(obj->s >= 0)
		close(obj->s);
	free(obj);
}
