upnpevents_removeSubscriber(const char * sid, int sidlen)
{
	struct subscriber * sub;
	if(!sid)
		return -1;
	for(sub = subscriberlist.lh_first; sub != NULL; sub = sub->entries.le_next) {
		if((sidlen == 41) && (memcmp(sid, sub->uuid, 41) == 0)) {
			if(sub->notify) {
				sub->notify->sub = NULL;
			}
			LIST_REMOVE(sub, entries);
			free(sub);
			return 0;
		}
	}
	return -1;
}
