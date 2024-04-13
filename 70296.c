daemon_msg_endcap_req(uint8 ver, struct daemon_slpars *pars,
    struct session *session)
{
	char errbuf[PCAP_ERRBUF_SIZE];		// buffer for network errors
	struct rpcap_header header;

	session_close(session);

	rpcap_createhdr(&header, ver, RPCAP_MSG_ENDCAP_REPLY, 0, 0);

	if (sock_send(pars->sockctrl, (char *) &header, sizeof(struct rpcap_header), errbuf, PCAP_ERRBUF_SIZE) == -1)
	{
		rpcapd_log(LOGPRIO_ERROR, "Send to client failed: %s", errbuf);
		return -1;
	}

	return 0;
}
