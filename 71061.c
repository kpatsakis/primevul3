void upnpevents_processfds(fd_set *readset, fd_set *writeset)
{
	struct upnp_event_notify * obj;
	struct upnp_event_notify * next;
	struct subscriber * sub;
	struct subscriber * subnext;
	time_t curtime;
	for(obj = notifylist.lh_first; obj != NULL; obj = obj->entries.le_next) {
		syslog(LOG_DEBUG, "%s: %p %d %d %d %d",
		       "upnpevents_processfds", obj, obj->state, obj->s,
		       FD_ISSET(obj->s, readset), FD_ISSET(obj->s, writeset));
		if(obj->s >= 0) {
			if(FD_ISSET(obj->s, readset) || FD_ISSET(obj->s, writeset))
				upnp_event_process_notify(obj);
		}
	}
	obj = notifylist.lh_first;
	while(obj != NULL) {
		next = obj->entries.le_next;
		if(obj->state == EError || obj->state == EFinished) {
			if(obj->s >= 0) {
				close(obj->s);
			}
			if(obj->sub)
				obj->sub->notify = NULL;
			/* remove also the subscriber from the list if there was an error */
			if(obj->state == EError && obj->sub) {
				syslog(LOG_ERR, "%s: %p, remove subscriber %s after an ERROR cb: %s",
				       "upnpevents_processfds", obj, obj->sub->uuid, obj->sub->callback);
				LIST_REMOVE(obj->sub, entries);
				free(obj->sub);
			}
			if(obj->buffer) {
				free(obj->buffer);
			}
			LIST_REMOVE(obj, entries);
			free(obj);
		}
		obj = next;
	}
	/* remove timeouted subscribers */
	curtime = upnp_time();
	for(sub = subscriberlist.lh_first; sub != NULL; ) {
		subnext = sub->entries.le_next;
		if(sub->timeout && curtime > sub->timeout && sub->notify == NULL) {
			syslog(LOG_INFO, "subscriber timeouted : %u > %u SID=%s",
			       (unsigned)curtime, (unsigned)sub->timeout, sub->uuid);
			LIST_REMOVE(sub, entries);
			free(sub);
		}
		sub = subnext;
	}
}
