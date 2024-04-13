void write_events_details(int s) {
	int n;
	char buff[80];
	struct upnp_event_notify * obj;
	struct subscriber * sub;
	write(s, "Events details :\n", 17);
	for(obj = notifylist.lh_first; obj != NULL; obj = obj->entries.le_next) {
		n = snprintf(buff, sizeof(buff), " %p sub=%p state=%d s=%d\n",
		             obj, obj->sub, obj->state, obj->s);
		write(s, buff, n);
	}
	write(s, "Subscribers :\n", 14);
	for(sub = subscriberlist.lh_first; sub != NULL; sub = sub->entries.le_next) {
		n = snprintf(buff, sizeof(buff), " %p timeout=%d seq=%u service=%d\n",
		             sub, (int)sub->timeout, sub->seq, sub->service);
		write(s, buff, n);
		n = snprintf(buff, sizeof(buff), "   notify=%p %s\n",
		             sub->notify, sub->uuid);
		write(s, buff, n);
		n = snprintf(buff, sizeof(buff), "   %s\n",
		             sub->callback);
		write(s, buff, n);
	}
}
