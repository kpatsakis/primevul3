daemon_msg_updatefilter_req(uint8 ver, struct daemon_slpars *pars,
    struct session *session, uint32 plen)
{
	char errbuf[PCAP_ERRBUF_SIZE];
	char errmsgbuf[PCAP_ERRBUF_SIZE];	// buffer for errors to send to the client
	int ret;				// status of daemon_unpackapplyfilter()
	struct rpcap_header header;		// keeps the answer to the updatefilter command

	ret = daemon_unpackapplyfilter(pars->sockctrl, session, &plen, errmsgbuf);
	if (ret == -1)
	{
		return -1;
	}
	if (ret == -2)
	{
		goto error;
	}

	if (rpcapd_discard(pars->sockctrl, plen) == -1)
	{
		return -1;
	}

	rpcap_createhdr(&header, ver, RPCAP_MSG_UPDATEFILTER_REPLY, 0, 0);

	if (sock_send(pars->sockctrl, (char *) &header, sizeof (struct rpcap_header), pcap_geterr(session->fp), PCAP_ERRBUF_SIZE))
	{
		rpcapd_log(LOGPRIO_ERROR, "Send to client failed: %s", errbuf);
		return -1;
	}

	return 0;

error:
	if (rpcapd_discard(pars->sockctrl, plen) == -1)
	{
		return -1;
	}
	rpcap_senderror(pars->sockctrl, ver, PCAP_ERR_UPDATEFILTER,
	    errmsgbuf, NULL);

	return 0;
}
