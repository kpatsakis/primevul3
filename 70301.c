daemon_msg_stats_req(uint8 ver, struct daemon_slpars *pars,
    struct session *session, uint32 plen, struct pcap_stat *stats,
    unsigned int svrcapt)
{
	char errbuf[PCAP_ERRBUF_SIZE];		// buffer for network errors
	char errmsgbuf[PCAP_ERRBUF_SIZE];	// buffer for errors to send to the client
	char sendbuf[RPCAP_NETBUF_SIZE];	// temporary buffer in which data to be sent is buffered
	int sendbufidx = 0;			// index which keeps the number of bytes currently buffered
	struct rpcap_stats *netstats;		// statistics sent on the network

	if (rpcapd_discard(pars->sockctrl, plen) == -1)
	{
		return -1;
	}

	if (sock_bufferize(NULL, sizeof(struct rpcap_header), NULL,
	    &sendbufidx, RPCAP_NETBUF_SIZE, SOCKBUF_CHECKONLY, errmsgbuf, PCAP_ERRBUF_SIZE) == -1)
		goto error;

	rpcap_createhdr((struct rpcap_header *) sendbuf, ver,
	    RPCAP_MSG_STATS_REPLY, 0, (uint16) sizeof(struct rpcap_stats));

	netstats = (struct rpcap_stats *) &sendbuf[sendbufidx];

	if (sock_bufferize(NULL, sizeof(struct rpcap_stats), NULL,
	    &sendbufidx, RPCAP_NETBUF_SIZE, SOCKBUF_CHECKONLY, errmsgbuf, PCAP_ERRBUF_SIZE) == -1)
		goto error;

	if (session && session->fp)
	{
		if (pcap_stats(session->fp, stats) == -1)
		{
			pcap_snprintf(errmsgbuf, PCAP_ERRBUF_SIZE, "%s", pcap_geterr(session->fp));
			goto error;
		}

		netstats->ifdrop = htonl(stats->ps_ifdrop);
		netstats->ifrecv = htonl(stats->ps_recv);
		netstats->krnldrop = htonl(stats->ps_drop);
		netstats->svrcapt = htonl(session->TotCapt);
	}
	else
	{
		netstats->ifdrop = htonl(stats->ps_ifdrop);
		netstats->ifrecv = htonl(stats->ps_recv);
		netstats->krnldrop = htonl(stats->ps_drop);
		netstats->svrcapt = htonl(svrcapt);
	}

	if (sock_send(pars->sockctrl, sendbuf, sendbufidx, errbuf, PCAP_ERRBUF_SIZE) == -1)
	{
		rpcapd_log(LOGPRIO_ERROR, "Send to client failed: %s", errbuf);
		return -1;
	}

	return 0;

error:
	rpcap_senderror(pars->sockctrl, ver, PCAP_ERR_GETSTATS,
	    errmsgbuf, NULL);
	return 0;
}
