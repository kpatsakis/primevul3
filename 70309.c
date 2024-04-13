rpcapd_recv_msg_header(SOCKET sock, struct rpcap_header *headerp)
{
	int nread;
	char errbuf[PCAP_ERRBUF_SIZE];		// buffer for network errors

	nread = sock_recv(sock, (char *) headerp, sizeof(struct rpcap_header),
	    SOCK_RECEIVEALL_YES|SOCK_EOF_ISNT_ERROR, errbuf, PCAP_ERRBUF_SIZE);
	if (nread == -1)
	{
		rpcapd_log(LOGPRIO_ERROR, "Read from client failed: %s", errbuf);
		return -1;
	}
	if (nread == 0)
	{
		return -2;
	}
	headerp->plen = ntohl(headerp->plen);
	return 0;
}
