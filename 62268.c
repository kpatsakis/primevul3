CMD_FUNC(m_svslogin)
{
	if (!SASL_SERVER || MyClient(sptr) || (parc < 3) || !parv[3])
		return 0;

	if (!stricmp(parv[1], me.name))
	{
		aClient *target_p;

		/* is the PUID valid? */
		if ((target_p = decode_puid(parv[2])) == NULL)
			return 0;

		if (target_p->user == NULL)
			make_user(target_p);

		strlcpy(target_p->user->svid, parv[3], sizeof(target_p->user->svid));

		sendto_one(target_p, err_str(RPL_LOGGEDIN), me.name,
			   BadPtr(target_p->name) ? "*" : target_p->name,
			   BadPtr(target_p->name) ? "*" : target_p->name,
			   BadPtr(target_p->user->username) ? "*" : target_p->user->username,
			   BadPtr(target_p->user->realhost) ? "*" : target_p->user->realhost,
			   target_p->user->svid, target_p->user->svid);

		return 0;
	}

	/* not for us; propagate. */
	sendto_server(cptr, 0, 0, ":%s SVSLOGIN %s %s %s",
	    sptr->name, parv[1], parv[2], parv[3]);

	return 0;
}
