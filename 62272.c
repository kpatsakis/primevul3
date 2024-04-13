static int abort_sasl(aClient *cptr)
{
	if (cptr->local->sasl_out == 0 || cptr->local->sasl_complete)
		return 0;

	cptr->local->sasl_out = cptr->local->sasl_complete = 0;
	sendto_one(cptr, err_str(ERR_SASLABORTED), me.name, BadPtr(cptr->name) ? "*" : cptr->name);

	if (*cptr->local->sasl_agent)
	{
		aClient *agent_p = find_client(cptr->local->sasl_agent, NULL);

		if (agent_p != NULL)
		{
			sendto_server(NULL, 0, 0, ":%s SASL %s %s D A",
			    me.name, AGENT_SID(agent_p), encode_puid(cptr));
			return 0;
		}
	}

	sendto_server(NULL, 0, 0, ":%s SASL * %s D A", me.name, encode_puid(cptr));
	return 0;
}
