upnpevents_renewSubscription(const char * sid, int sidlen, int timeout)
{
	struct subscriber * sub;
	for(sub = subscriberlist.lh_first; sub != NULL; sub = sub->entries.le_next) {
		if((sidlen == 41) && (memcmp(sid, sub->uuid, 41) == 0)) {
#ifdef UPNP_STRICT
			/* check if the subscription already timeouted */
			if(sub->timeout && upnp_time() > sub->timeout)
				continue;
#endif
			sub->timeout = (timeout ? upnp_time() + timeout : 0);
			return sub->uuid;
		}
	}
	return NULL;
}
