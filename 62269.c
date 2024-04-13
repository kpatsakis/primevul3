CMD_FUNC(m_sasl)
{
	if (!SASL_SERVER || MyClient(sptr) || (parc < 4) || !parv[4])
		return 0;

	if (!stricmp(parv[1], me.name))
	{
		aClient *target_p;

		/* is the PUID valid? */
		if ((target_p = decode_puid(parv[2])) == NULL)
			return 0;

		if (target_p->user == NULL)
			make_user(target_p);

		/* reject if another SASL agent is answering */
		if (*target_p->local->sasl_agent && stricmp(sptr->name, target_p->local->sasl_agent))
			return 0;
		else
			strlcpy(target_p->local->sasl_agent, sptr->name, sizeof(target_p->local->sasl_agent));

		if (*parv[3] == 'C')
			sendto_one(target_p, "AUTHENTICATE %s", parv[4]);
		else if (*parv[3] == 'D')
		{
			if (*parv[4] == 'F')
				sendto_one(target_p, err_str(ERR_SASLFAIL), me.name, BadPtr(target_p->name) ? "*" : target_p->name);
			else if (*parv[4] == 'S')
			{
				target_p->local->sasl_complete++;
				sendto_one(target_p, err_str(RPL_SASLSUCCESS), me.name, BadPtr(target_p->name) ? "*" : target_p->name);
			}

			*target_p->local->sasl_agent = '\0';
		}
		else if (*parv[3] == 'M')
			sendto_one(target_p, err_str(RPL_SASLMECHS), me.name, BadPtr(target_p->name) ? "*" : target_p->name, parv[4]);

		return 0;
	}

	/* not for us; propagate. */
	sendto_server(cptr, 0, 0, ":%s SASL %s %s %c %s %s",
	    sptr->name, parv[1], parv[2], *parv[3], parv[4], parc > 5 ? parv[5] : "");

	return 0;
}
