upnp_event_process_notify(struct upnp_event_notify * obj)
{
	int err;
	socklen_t len;
	switch(obj->state) {
	case EConnecting:
		/* now connected or failed to connect */
		len = sizeof(err);
		if(getsockopt(obj->s, SOL_SOCKET, SO_ERROR, &err, &len) < 0) {
			syslog(LOG_ERR, "%s: getsockopt: %m", "upnp_event_process_notify");
			obj->state = EError;
			break;
		}
		if(err != 0) {
			errno = err;
			syslog(LOG_WARNING, "%s: connect(%s%s): %m",
			       "upnp_event_process_notify",
			       obj->addrstr, obj->portstr);
			obj->state = EError;
			break;
		}
		upnp_event_prepare(obj);
		if(obj->state == ESending)
			upnp_event_send(obj);
		break;
	case ESending:
		upnp_event_send(obj);
		break;
	case EWaitingForResponse:
		upnp_event_recv(obj);
		break;
	case EFinished:
		close(obj->s);
		obj->s = -1;
		break;
	default:
		syslog(LOG_ERR, "%s: unknown state", "upnp_event_process_notify");
	}
}
