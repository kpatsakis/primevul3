upnp_event_var_change_notify(enum subscriber_service_enum service)
{
	struct subscriber * sub;
	for(sub = subscriberlist.lh_first; sub != NULL; sub = sub->entries.le_next) {
		if(sub->service == service && sub->notify == NULL)
			upnp_event_create_notify(sub);
	}
}
