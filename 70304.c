daemon_thrdatamain(void *ptr)
{
	char errbuf[PCAP_ERRBUF_SIZE + 1];	// error buffer
	struct session *session;		// pointer to the struct session for this session
	int retval;							// general variable used to keep the return value of other functions
	struct rpcap_pkthdr *net_pkt_header;// header of the packet
	struct pcap_pkthdr *pkt_header;		// pointer to the buffer that contains the header of the current packet
	u_char *pkt_data;					// pointer to the buffer that contains the current packet
	size_t sendbufsize;			// size for the send buffer
	char *sendbuf;						// temporary buffer in which data to be sent is buffered
	int sendbufidx;						// index which keeps the number of bytes currently buffered
	int status;
#ifndef _WIN32
	sigset_t sigusr1;			// signal set with just SIGUSR1
#endif

	session = (struct session *) ptr;

	session->TotCapt = 0;			// counter which is incremented each time a packet is received

	memset(errbuf, 0, sizeof(errbuf));

	if (pcap_snapshot(session->fp) < 0)
	{
		rpcapd_log(LOGPRIO_ERROR,
		    "Unable to allocate the buffer for this child thread: snapshot length of %d is negative",
		        pcap_snapshot(session->fp));
		sendbuf = NULL;	// we can't allocate a buffer, so nothing to free
		goto error;
	}
	sendbufsize = sizeof(struct rpcap_header) + sizeof(struct rpcap_pkthdr) + pcap_snapshot(session->fp);
	sendbuf = (char *) malloc (sendbufsize);
	if (sendbuf == NULL)
	{
		rpcapd_log(LOGPRIO_ERROR,
		    "Unable to allocate the buffer for this child thread");
		goto error;
	}

#ifndef _WIN32
	sigemptyset(&sigusr1);
	sigaddset(&sigusr1, SIGUSR1);
	pthread_sigmask(SIG_BLOCK, &sigusr1, NULL);
#endif

	for (;;)
	{
#ifndef _WIN32
		pthread_sigmask(SIG_UNBLOCK, &sigusr1, NULL);
#endif
		retval = pcap_next_ex(session->fp, &pkt_header, (const u_char **) &pkt_data);	// cast to avoid a compiler warning
#ifndef _WIN32
		pthread_sigmask(SIG_BLOCK, &sigusr1, NULL);
#endif
		if (retval < 0)
			break;		// error
		if (retval == 0)	// Read timeout elapsed
			continue;

		sendbufidx = 0;

		if (sock_bufferize(NULL, sizeof(struct rpcap_header), NULL,
		    &sendbufidx, sendbufsize, SOCKBUF_CHECKONLY, errbuf,
		    PCAP_ERRBUF_SIZE) == -1)
		{
			rpcapd_log(LOGPRIO_ERROR,
			    "sock_bufferize() error sending packet message: %s",
			    errbuf);
			goto error;
		}

		rpcap_createhdr((struct rpcap_header *) sendbuf,
		    session->protocol_version, RPCAP_MSG_PACKET, 0,
		    (uint16) (sizeof(struct rpcap_pkthdr) + pkt_header->caplen));

		net_pkt_header = (struct rpcap_pkthdr *) &sendbuf[sendbufidx];

		if (sock_bufferize(NULL, sizeof(struct rpcap_pkthdr), NULL,
		    &sendbufidx, sendbufsize, SOCKBUF_CHECKONLY, errbuf,
		    PCAP_ERRBUF_SIZE) == -1)
		{
			rpcapd_log(LOGPRIO_ERROR,
			    "sock_bufferize() error sending packet message: %s",
			    errbuf);
			goto error;
		}

		net_pkt_header->caplen = htonl(pkt_header->caplen);
		net_pkt_header->len = htonl(pkt_header->len);
		net_pkt_header->npkt = htonl(++(session->TotCapt));
		net_pkt_header->timestamp_sec = htonl(pkt_header->ts.tv_sec);
		net_pkt_header->timestamp_usec = htonl(pkt_header->ts.tv_usec);

		if (sock_bufferize((char *) pkt_data, pkt_header->caplen,
		    sendbuf, &sendbufidx, sendbufsize, SOCKBUF_BUFFERIZE,
		    errbuf, PCAP_ERRBUF_SIZE) == -1)
		{
			rpcapd_log(LOGPRIO_ERROR,
			    "sock_bufferize() error sending packet message: %s",
			    errbuf);
			goto error;
		}

		status = sock_send(session->sockdata, sendbuf, sendbufidx, errbuf, PCAP_ERRBUF_SIZE);
		if (status < 0)
		{
			if (status == -1)
			{
				rpcapd_log(LOGPRIO_ERROR,
				    "Send of packet to client failed: %s",
				    errbuf);
			}

			goto error;
		}
	}

	if (retval < 0 && retval != PCAP_ERROR_BREAK)
	{
		pcap_snprintf(errbuf, PCAP_ERRBUF_SIZE, "Error reading the packets: %s", pcap_geterr(session->fp));
		rpcap_senderror(session->sockctrl, session->protocol_version,
		    PCAP_ERR_READEX, errbuf, NULL);
	}

error:
	free(sendbuf);

	return 0;
}
