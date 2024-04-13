get_upnp_rules_state_list(int max_rules_number_target)
{
	/*char ifname[IFNAMSIZ];*/
	int proto;
	unsigned short iport;
	unsigned int timestamp;
	struct rule_state * tmp;
	struct rule_state * list = 0;
	struct rule_state * * p;
	int i = 0;
	time_t current_time;

	/*ifname[0] = '\0';*/
	tmp = malloc(sizeof(struct rule_state));
	if(!tmp)
		return 0;
	current_time = upnp_time();
	nextruletoclean_timestamp = 0;
	while(get_redirect_rule_by_index(i, /*ifname*/0, &tmp->eport, 0, 0,
	                              &iport, &proto, 0, 0, 0,0, &timestamp,
								  &tmp->packets, &tmp->bytes) >= 0)
	{
		tmp->to_remove = 0;
		if(timestamp > 0) {
			/* need to remove this port mapping ? */
			if(timestamp <= (unsigned int)current_time)
				tmp->to_remove = 1;
			else if((nextruletoclean_timestamp <= (unsigned int)current_time)
			       || (timestamp < nextruletoclean_timestamp))
				nextruletoclean_timestamp = timestamp;
		}
		tmp->proto = (short)proto;
		/* add tmp to list */
		tmp->next = list;
		list = tmp;
		/* prepare next iteration */
		i++;
		tmp = malloc(sizeof(struct rule_state));
		if(!tmp)
			break;
	}
#ifdef PCP_PEER
	i=0;
	while(get_peer_rule_by_index(i, /*ifname*/0, &tmp->eport, 0, 0,
		                              &iport, &proto, 0, 0, 0,0,0, &timestamp,
									  &tmp->packets, &tmp->bytes) >= 0)
	{
		tmp->to_remove = 0;
		if(timestamp > 0) {
			/* need to remove this port mapping ? */
			if(timestamp <= (unsigned int)current_time)
				tmp->to_remove = 1;
			else if((nextruletoclean_timestamp <= (unsigned int)current_time)
				   || (timestamp < nextruletoclean_timestamp))
				nextruletoclean_timestamp = timestamp;
		}
		tmp->proto = (short)proto;
		/* add tmp to list */
		tmp->next = list;
		list = tmp;
		/* prepare next iteration */
		i++;
		tmp = malloc(sizeof(struct rule_state));
		if(!tmp)
			break;
	}
#endif
	free(tmp);
	/* remove the redirections that need to be removed */
	for(p = &list, tmp = list; tmp; tmp = *p)
	{
		if(tmp->to_remove)
		{
			syslog(LOG_NOTICE, "remove port mapping %hu %s because it has expired",
			       tmp->eport, proto_itoa(tmp->proto));
			_upnp_delete_redir(tmp->eport, tmp->proto);
			*p = tmp->next;
			free(tmp);
			i--;
		} else {
			p = &(tmp->next);
		}
	}
	/* return empty list if not enough redirections */
	if(i<=max_rules_number_target)
		while(list)
		{
			tmp = list;
			list = tmp->next;
			free(tmp);
		}
	/* return list */
	return list;
}
