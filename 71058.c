static void upnp_event_recv(struct upnp_event_notify * obj)
{
	int n;
	n = recv(obj->s, obj->buffer, obj->buffersize, 0);
	if(n<0) {
		if(errno != EAGAIN &&
		   errno != EWOULDBLOCK &&
		   errno != EINTR) {
			syslog(LOG_ERR, "%s: recv(): %m", "upnp_event_recv");
			obj->state = EError;
		}
		return;
	}
	syslog(LOG_DEBUG, "%s: (%dbytes) %.*s", "upnp_event_recv",
	       n, n, obj->buffer);
	/* TODO : do something with the data recevied ?
	 * right now, n (number of bytes received) is ignored
	 * We may need to recv() more bytes. */
	obj->state = EFinished;
	if(obj->sub)
		obj->sub->seq++;
}
