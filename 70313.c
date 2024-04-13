daemon_serviceloop(SOCKET sockctrl, int isactive, char *passiveClients,
    int nullAuthAllowed)
{
	struct daemon_slpars pars;		// service loop parameters
	char errbuf[PCAP_ERRBUF_SIZE + 1];	// keeps the error string, prior to be printed
	char errmsgbuf[PCAP_ERRBUF_SIZE + 1];	// buffer for errors to send to the client
	int host_port_check_status;
	int nrecv;
	struct rpcap_header header;		// RPCAP message general header
	uint32 plen;				// payload length from header
	int authenticated = 0;			// 1 if the client has successfully authenticated
	char source[PCAP_BUF_SIZE+1];		// keeps the string that contains the interface to open
	int got_source = 0;			// 1 if we've gotten the source from an open request
#ifndef _WIN32
	struct sigaction action;
#endif
	struct session *session = NULL;		// struct session main variable
	const char *msg_type_string;		// string for message type
	int client_told_us_to_close = 0;	// 1 if the client told us to close the capture

	struct pcap_stat stats;
	unsigned int svrcapt;

	struct rpcap_sampling samp_param;	// in case sampling has been requested

	fd_set rfds;				// set of socket descriptors we have to check
	struct timeval tv;			// maximum time the select() can block waiting for data
	int retval;				// select() return value

	*errbuf = 0;	// Initialize errbuf

	pars.sockctrl = sockctrl;
	pars.isactive = isactive;		// active mode
	pars.nullAuthAllowed = nullAuthAllowed;

	if (pars.isactive)
	{
		free(passiveClients);
		passiveClients = NULL;
	}
	else
	{
		struct sockaddr_storage from;
		socklen_t fromlen;

		fromlen = sizeof(struct sockaddr_storage);
		if (getpeername(pars.sockctrl, (struct sockaddr *)&from,
		    &fromlen) == -1)
		{
			sock_geterror("getpeername()", errmsgbuf, PCAP_ERRBUF_SIZE);
			if (rpcap_senderror(pars.sockctrl, 0, PCAP_ERR_NETW, errmsgbuf, errbuf) == -1)
				rpcapd_log(LOGPRIO_ERROR, "Send to client failed: %s", errbuf);
			goto end;
		}

		host_port_check_status = sock_check_hostlist(passiveClients, RPCAP_HOSTLIST_SEP, &from, errmsgbuf, PCAP_ERRBUF_SIZE);
		free(passiveClients);
		passiveClients = NULL;
		if (host_port_check_status < 0)
		{
			if (host_port_check_status == -2) {
				rpcapd_log(LOGPRIO_ERROR, "%s", errmsgbuf);
			}

			if (rpcap_senderror(pars.sockctrl, 0, PCAP_ERR_HOSTNOAUTH, errmsgbuf, errbuf) == -1)
				rpcapd_log(LOGPRIO_ERROR, "Send to client failed: %s", errbuf);
			goto end;
		}
	}

#ifndef _WIN32
	memset(&action, 0, sizeof (action));
	action.sa_handler = noop_handler;
	action.sa_flags = 0;
	sigemptyset(&action.sa_mask);
	sigaction(SIGUSR1, &action, NULL);
#endif

	while (!authenticated)
	{
		if (!pars.isactive)
		{
			FD_ZERO(&rfds);
			tv.tv_sec = RPCAP_TIMEOUT_INIT;
			tv.tv_usec = 0;

			FD_SET(pars.sockctrl, &rfds);

			retval = select(pars.sockctrl + 1, &rfds, NULL, NULL, &tv);
			if (retval == -1)
			{
				sock_geterror("select() failed", errmsgbuf, PCAP_ERRBUF_SIZE);
				if (rpcap_senderror(pars.sockctrl, 0, PCAP_ERR_NETW, errmsgbuf, errbuf) == -1)
					rpcapd_log(LOGPRIO_ERROR, "Send to client failed: %s", errbuf);
				goto end;
			}

			if (retval == 0)
			{
				if (rpcap_senderror(pars.sockctrl, 0, PCAP_ERR_INITTIMEOUT, "The RPCAP initial timeout has expired", errbuf) == -1)
					rpcapd_log(LOGPRIO_ERROR, "Send to client failed: %s", errbuf);
				goto end;
			}
		}

		nrecv = rpcapd_recv_msg_header(pars.sockctrl, &header);
		if (nrecv == -1)
		{
			goto end;
		}
		if (nrecv == -2)
		{
			goto end;
		}

		plen = header.plen;

		if (header.ver != 0)
		{
			if (rpcap_senderror(pars.sockctrl, header.ver,
			    PCAP_ERR_WRONGVER,
			    "RPCAP version in requests in the authentication phase must be 0",
			    errbuf) == -1)
			{
				rpcapd_log(LOGPRIO_ERROR, "Send to client failed: %s", errbuf);
				goto end;
			}

			(void)rpcapd_discard(pars.sockctrl, plen);
			goto end;
		}

		switch (header.type)
		{
			case RPCAP_MSG_AUTH_REQ:
				retval = daemon_msg_auth_req(&pars, plen);
				if (retval == -1)
				{
					goto end;
				}
				if (retval == -2)
				{
					continue;
				}

				authenticated = 1;
				break;

			case RPCAP_MSG_CLOSE:
				(void)rpcapd_discard(pars.sockctrl, plen);
				goto end;

			case RPCAP_MSG_ERROR:
				(void)daemon_msg_err(pars.sockctrl, plen);
				goto end;

			case RPCAP_MSG_FINDALLIF_REQ:
			case RPCAP_MSG_OPEN_REQ:
			case RPCAP_MSG_STARTCAP_REQ:
			case RPCAP_MSG_UPDATEFILTER_REQ:
			case RPCAP_MSG_STATS_REQ:
			case RPCAP_MSG_ENDCAP_REQ:
			case RPCAP_MSG_SETSAMPLING_REQ:
				msg_type_string = rpcap_msg_type_string(header.type);
				if (msg_type_string != NULL)
				{
					pcap_snprintf(errmsgbuf, PCAP_ERRBUF_SIZE, "%s request sent before authentication was completed", msg_type_string);
				}
				else
				{
					pcap_snprintf(errmsgbuf, PCAP_ERRBUF_SIZE, "Message of type %u sent before authentication was completed", header.type);
				}
				if (rpcap_senderror(pars.sockctrl, header.ver,
				    PCAP_ERR_WRONGMSG, errmsgbuf, errbuf) == -1)
				{
					rpcapd_log(LOGPRIO_ERROR, "Send to client failed: %s", errbuf);
					goto end;
				}
				if (rpcapd_discard(pars.sockctrl, plen) == -1)
				{
					goto end;
				}
				break;

			case RPCAP_MSG_PACKET:
			case RPCAP_MSG_FINDALLIF_REPLY:
			case RPCAP_MSG_OPEN_REPLY:
			case RPCAP_MSG_STARTCAP_REPLY:
			case RPCAP_MSG_UPDATEFILTER_REPLY:
			case RPCAP_MSG_AUTH_REPLY:
			case RPCAP_MSG_STATS_REPLY:
			case RPCAP_MSG_ENDCAP_REPLY:
			case RPCAP_MSG_SETSAMPLING_REPLY:
				msg_type_string = rpcap_msg_type_string(header.type);
				if (msg_type_string != NULL)
				{
					pcap_snprintf(errmsgbuf, PCAP_ERRBUF_SIZE, "Server-to-client message %s received from client", msg_type_string);
				}
				else
				{
					pcap_snprintf(errmsgbuf, PCAP_ERRBUF_SIZE, "Server-to-client message of type %u received from client", header.type);
				}
				if (rpcap_senderror(pars.sockctrl, header.ver,
				    PCAP_ERR_WRONGMSG, errmsgbuf, errbuf) == -1)
				{
					rpcapd_log(LOGPRIO_ERROR, "Send to client failed: %s", errbuf);
					goto end;
				}
				if (rpcapd_discard(pars.sockctrl, plen) == -1)
				{
					goto end;
				}
				break;

			default:
				pcap_snprintf(errmsgbuf, PCAP_ERRBUF_SIZE, "Unknown message type %u", header.type);
				if (rpcap_senderror(pars.sockctrl, header.ver,
				    PCAP_ERR_WRONGMSG, errmsgbuf, errbuf) == -1)
				{
					rpcapd_log(LOGPRIO_ERROR, "Send to client failed: %s", errbuf);
					goto end;
				}
				if (rpcapd_discard(pars.sockctrl, plen) == -1)
				{
					goto end;
				}
				break;
		}
	}


	stats.ps_ifdrop = 0;
	stats.ps_recv = 0;
	stats.ps_drop = 0;
	svrcapt = 0;

	for (;;)
	{
		errbuf[0] = 0;	// clear errbuf

		if ((!pars.isactive) &&  ((session == NULL) || ((session != NULL) && (session->sockdata == 0))))
		{
			FD_ZERO(&rfds);
			tv.tv_sec = RPCAP_TIMEOUT_RUNTIME;
			tv.tv_usec = 0;

			FD_SET(pars.sockctrl, &rfds);

			retval = select(pars.sockctrl + 1, &rfds, NULL, NULL, &tv);
			if (retval == -1)
			{
				sock_geterror("select() failed", errmsgbuf, PCAP_ERRBUF_SIZE);
				if (rpcap_senderror(pars.sockctrl, 0,
				    PCAP_ERR_NETW, errmsgbuf, errbuf) == -1)
					rpcapd_log(LOGPRIO_ERROR, "Send to client failed: %s", errbuf);
				goto end;
			}

			if (retval == 0)
			{
				if (rpcap_senderror(pars.sockctrl, 0,
				    PCAP_ERR_INITTIMEOUT,
				    "The RPCAP initial timeout has expired",
				    errbuf) == -1)
					rpcapd_log(LOGPRIO_ERROR, "Send to client failed: %s", errbuf);
				goto end;
			}
		}

		nrecv = rpcapd_recv_msg_header(pars.sockctrl, &header);
		if (nrecv == -1)
		{
			goto end;
		}
		if (nrecv == -2)
		{
			goto end;
		}

		plen = header.plen;

		if (!RPCAP_VERSION_IS_SUPPORTED(header.ver))
		{
			if (rpcap_senderror(pars.sockctrl,
			    header.ver, PCAP_ERR_WRONGVER,
			    "RPCAP version in message isn't supported by the server",
			    errbuf) == -1)
			{
				rpcapd_log(LOGPRIO_ERROR, "Send to client failed: %s", errbuf);
				goto end;
			}

			(void)rpcapd_discard(pars.sockctrl, plen);
			goto end;
		}

		switch (header.type)
		{
			case RPCAP_MSG_ERROR:		// The other endpoint reported an error
			{
				(void)daemon_msg_err(pars.sockctrl, plen);
				break;
			}

			case RPCAP_MSG_FINDALLIF_REQ:
			{
				if (daemon_msg_findallif_req(header.ver, &pars, plen) == -1)
				{
					goto end;
				}
				break;
			}

			case RPCAP_MSG_OPEN_REQ:
			{
				retval = daemon_msg_open_req(header.ver, &pars,
				    plen, source, sizeof(source));
				if (retval == -1)
				{
					goto end;
				}
				got_source = 1;
				break;
			}

			case RPCAP_MSG_STARTCAP_REQ:
			{
				if (!got_source)
				{
					if (rpcap_senderror(pars.sockctrl,
					    header.ver,
					    PCAP_ERR_STARTCAPTURE,
					    "No capture device was specified",
					    errbuf) == -1)
					{
						rpcapd_log(LOGPRIO_ERROR, "Send to client failed: %s", errbuf);
						goto end;
					}
					if (rpcapd_discard(pars.sockctrl, plen) == -1)
					{
						goto end;
					}
					break;
				}

				if (daemon_msg_startcap_req(header.ver, &pars,
				    plen, source, &session, &samp_param) == -1)
				{
					goto end;
				}
				break;
			}

			case RPCAP_MSG_UPDATEFILTER_REQ:
			{
				if (session)
				{
					if (daemon_msg_updatefilter_req(header.ver,
					    &pars, session, plen) == -1)
					{
						goto end;
					}
				}
				else
				{
					if (rpcap_senderror(pars.sockctrl,
					    header.ver, PCAP_ERR_UPDATEFILTER,
					    "Device not opened. Cannot update filter",
					    errbuf) == -1)
					{
						rpcapd_log(LOGPRIO_ERROR, "Send to client failed: %s", errbuf);
						goto end;
					}
				}
				break;
			}

			case RPCAP_MSG_CLOSE:		// The other endpoint close the pcap session
			{
				client_told_us_to_close = 1;
				rpcapd_log(LOGPRIO_DEBUG, "The other end system asked to close the connection.");
				goto end;
			}

			case RPCAP_MSG_STATS_REQ:
			{
				if (daemon_msg_stats_req(header.ver, &pars,
				    session, plen, &stats, svrcapt) == -1)
				{
					goto end;
				}
				break;
			}

			case RPCAP_MSG_ENDCAP_REQ:		// The other endpoint close the current capture session
			{
				if (session)
				{
					if (pcap_stats(session->fp, &stats))
					{
						svrcapt = session->TotCapt;
					}
					else
					{
						stats.ps_ifdrop = 0;
						stats.ps_recv = 0;
						stats.ps_drop = 0;
						svrcapt = 0;
					}

					if (daemon_msg_endcap_req(header.ver,
					    &pars, session) == -1)
					{
						free(session);
						session = NULL;
						goto end;
					}
					free(session);
					session = NULL;
				}
				else
				{
					rpcap_senderror(pars.sockctrl,
					    header.ver, PCAP_ERR_ENDCAPTURE,
					    "Device not opened. Cannot close the capture",
					    errbuf);
				}
				break;
			}

			case RPCAP_MSG_SETSAMPLING_REQ:
			{
				if (daemon_msg_setsampling_req(header.ver,
				    &pars, plen, &samp_param) == -1)
				{
					goto end;
				}
				break;
			}

			case RPCAP_MSG_AUTH_REQ:
			{
				rpcapd_log(LOGPRIO_INFO, "The client sent an RPCAP_MSG_AUTH_REQ message after authentication was completed");
				if (rpcap_senderror(pars.sockctrl, header.ver,
				    PCAP_ERR_WRONGMSG,
				    "RPCAP_MSG_AUTH_REQ request sent after authentication was completed",
				    errbuf) == -1)
				{
					rpcapd_log(LOGPRIO_ERROR, "Send to client failed: %s", errbuf);
					goto end;
				}
				if (rpcapd_discard(pars.sockctrl, plen) == -1)
				{
					goto end;
				}
				goto end;

			case RPCAP_MSG_PACKET:
			case RPCAP_MSG_FINDALLIF_REPLY:
			case RPCAP_MSG_OPEN_REPLY:
			case RPCAP_MSG_STARTCAP_REPLY:
			case RPCAP_MSG_UPDATEFILTER_REPLY:
			case RPCAP_MSG_AUTH_REPLY:
			case RPCAP_MSG_STATS_REPLY:
			case RPCAP_MSG_ENDCAP_REPLY:
			case RPCAP_MSG_SETSAMPLING_REPLY:
				msg_type_string = rpcap_msg_type_string(header.type);
				if (msg_type_string != NULL)
				{
					rpcapd_log(LOGPRIO_INFO, "The client sent a %s server-to-client message", msg_type_string);
					pcap_snprintf(errmsgbuf, PCAP_ERRBUF_SIZE, "Server-to-client message %s received from client", msg_type_string);
				}
				else
				{
					rpcapd_log(LOGPRIO_INFO, "The client sent a server-to-client message of type %u", header.type);
					pcap_snprintf(errmsgbuf, PCAP_ERRBUF_SIZE, "Server-to-client message of type %u received from client", header.type);
				}
				if (rpcap_senderror(pars.sockctrl, header.ver,
				    PCAP_ERR_WRONGMSG, errmsgbuf, errbuf) == -1)
				{
					rpcapd_log(LOGPRIO_ERROR, "Send to client failed: %s", errbuf);
					goto end;
				}
				if (rpcapd_discard(pars.sockctrl, plen) == -1)
				{
					goto end;
				}
				goto end;

			default:
				rpcapd_log(LOGPRIO_INFO, "The client sent a message of type %u", header.type);
				pcap_snprintf(errmsgbuf, PCAP_ERRBUF_SIZE, "Unknown message type %u", header.type);
				if (rpcap_senderror(pars.sockctrl, header.ver,
				    PCAP_ERR_WRONGMSG, errbuf, errmsgbuf) == -1)
				{
					rpcapd_log(LOGPRIO_ERROR, "Send to client failed: %s", errbuf);
					goto end;
				}
				if (rpcapd_discard(pars.sockctrl, plen) == -1)
				{
					goto end;
				}
				goto end;
			}
		}
	}

end:
	if (session)
	{
		session_close(session);
		free(session);
		session = NULL;
	}

	sock_close(sockctrl, NULL, 0);

	rpcapd_log(LOGPRIO_DEBUG, "I'm exiting from the child loop");

	return client_told_us_to_close;
}
