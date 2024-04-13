rpcapd_recv(SOCKET sock, char *buffer, size_t toread, uint32 *plen, char *errmsgbuf)
{
	int nread;
	char errbuf[PCAP_ERRBUF_SIZE];		// buffer for network errors

	if (toread > *plen)
	{
		pcap_snprintf(errmsgbuf, PCAP_ERRBUF_SIZE, "Message payload is too short");
		return -2;
	}
	nread = sock_recv(sock, buffer, toread,
	    SOCK_RECEIVEALL_YES|SOCK_EOF_IS_ERROR, errbuf, PCAP_ERRBUF_SIZE);
	if (nread == -1)
	{
		rpcapd_log(LOGPRIO_ERROR, "Read from client failed: %s", errbuf);
		return -1;
	}
	*plen -= nread;
	return 0;
}
