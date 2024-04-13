daemon_msg_setsampling_req(uint8 ver, struct daemon_slpars *pars, uint32 plen,
    struct rpcap_sampling *samp_param)
{
	char errbuf[PCAP_ERRBUF_SIZE];		// buffer for network errors
	char errmsgbuf[PCAP_ERRBUF_SIZE];
	struct rpcap_header header;
	struct rpcap_sampling rpcap_samp;
	int status;

	status = rpcapd_recv(pars->sockctrl, (char *) &rpcap_samp, sizeof(struct rpcap_sampling), &plen, errmsgbuf);
	if (status == -1)
	{
		return -1;
	}
	if (status == -2)
	{
		goto error;
	}

	samp_param->method = rpcap_samp.method;
	samp_param->value = ntohl(rpcap_samp.value);

	rpcap_createhdr(&header, ver, RPCAP_MSG_SETSAMPLING_REPLY, 0, 0);

	if (sock_send(pars->sockctrl, (char *) &header, sizeof (struct rpcap_header), errbuf, PCAP_ERRBUF_SIZE) == -1)
	{
		rpcapd_log(LOGPRIO_ERROR, "Send to client failed: %s", errbuf);
		return -1;
	}

	if (rpcapd_discard(pars->sockctrl, plen) == -1)
	{
		return -1;
	}

	return 0;

error:
	if (rpcap_senderror(pars->sockctrl, ver, PCAP_ERR_SETSAMPLING,
	    errmsgbuf, errbuf) == -1)
	{
		rpcapd_log(LOGPRIO_ERROR, "Send to client failed: %s", errbuf);
		return -1;
	}

	if (rpcapd_discard(pars->sockctrl, plen) == -1)
	{
		return -1;
	}

	return 0;
}
