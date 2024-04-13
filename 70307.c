rpcapd_discard(SOCKET sock, uint32 len)
{
	char errbuf[PCAP_ERRBUF_SIZE + 1];	// keeps the error string, prior to be printed

	if (len != 0)
	{
		if (sock_discard(sock, len, errbuf, PCAP_ERRBUF_SIZE) == -1)
		{
			rpcapd_log(LOGPRIO_ERROR, "Read from client failed: %s", errbuf);
			return -1;
		}
	}
	return 0;
}
