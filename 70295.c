daemon_msg_auth_req(struct daemon_slpars *pars, uint32 plen)
{
	char errbuf[PCAP_ERRBUF_SIZE];		// buffer for network errors
	char errmsgbuf[PCAP_ERRBUF_SIZE];	// buffer for errors to send to the client
	int status;
	struct rpcap_auth auth;			// RPCAP authentication header
	char sendbuf[RPCAP_NETBUF_SIZE];	// temporary buffer in which data to be sent is buffered
	int sendbufidx = 0;			// index which keeps the number of bytes currently buffered
	struct rpcap_authreply *authreply;	// authentication reply message

	status = rpcapd_recv(pars->sockctrl, (char *) &auth, sizeof(struct rpcap_auth), &plen, errmsgbuf);
	if (status == -1)
	{
		return -1;
	}
	if (status == -2)
	{
		goto error;
	}

	switch (ntohs(auth.type))
	{
		case RPCAP_RMTAUTH_NULL:
		{
			if (!pars->nullAuthAllowed)
			{
				pcap_snprintf(errmsgbuf, PCAP_ERRBUF_SIZE,
				    "Authentication failed; NULL authentication not permitted.");
				if (rpcap_senderror(pars->sockctrl, 0,
				    PCAP_ERR_AUTH_FAILED, errmsgbuf, errbuf) == -1)
				{
					rpcapd_log(LOGPRIO_ERROR, "Send to client failed: %s", errbuf);
					return -1;
				}
				goto error_noreply;
			}
			break;
		}

		case RPCAP_RMTAUTH_PWD:
		{
			char *username, *passwd;
			uint32 usernamelen, passwdlen;

			usernamelen = ntohs(auth.slen1);
			username = (char *) malloc (usernamelen + 1);
			if (username == NULL)
			{
				pcap_fmt_errmsg_for_errno(errmsgbuf,
				    PCAP_ERRBUF_SIZE, errno, "malloc() failed");
				goto error;
			}
			status = rpcapd_recv(pars->sockctrl, username, usernamelen, &plen, errmsgbuf);
			if (status == -1)
			{
				free(username);
				return -1;
			}
			if (status == -2)
			{
				free(username);
				goto error;
			}
			username[usernamelen] = '\0';

			passwdlen = ntohs(auth.slen2);
			passwd = (char *) malloc (passwdlen + 1);
			if (passwd == NULL)
			{
				pcap_fmt_errmsg_for_errno(errmsgbuf,
				    PCAP_ERRBUF_SIZE, errno, "malloc() failed");
				free(username);
				goto error;
			}
			status = rpcapd_recv(pars->sockctrl, passwd, passwdlen, &plen, errmsgbuf);
			if (status == -1)
			{
				free(username);
				free(passwd);
				return -1;
			}
			if (status == -2)
			{
				free(username);
				free(passwd);
				goto error;
			}
			passwd[passwdlen] = '\0';

			if (daemon_AuthUserPwd(username, passwd, errmsgbuf))
			{
				free(username);
				free(passwd);
				if (rpcap_senderror(pars->sockctrl, 0,
				    PCAP_ERR_AUTH_FAILED, errmsgbuf, errbuf) == -1)
				{
					rpcapd_log(LOGPRIO_ERROR, "Send to client failed: %s", errbuf);
					return -1;
				}

				sleep_secs(RPCAP_SUSPEND_WRONGAUTH);
				goto error_noreply;
			}

			free(username);
			free(passwd);
			break;
			}

		default:
			pcap_snprintf(errmsgbuf, PCAP_ERRBUF_SIZE,
			    "Authentication type not recognized.");
			if (rpcap_senderror(pars->sockctrl, 0,
			    PCAP_ERR_AUTH_TYPE_NOTSUP, errmsgbuf, errbuf) == -1)
			{
				rpcapd_log(LOGPRIO_ERROR, "Send to client failed: %s", errbuf);
				return -1;
			}
			goto error_noreply;
	}

	if (sock_bufferize(NULL, sizeof(struct rpcap_header), NULL, &sendbufidx,
	    RPCAP_NETBUF_SIZE, SOCKBUF_CHECKONLY, errmsgbuf, PCAP_ERRBUF_SIZE) == -1)
		goto error;

	rpcap_createhdr((struct rpcap_header *) sendbuf, 0,
	    RPCAP_MSG_AUTH_REPLY, 0, sizeof(struct rpcap_authreply));

	authreply = (struct rpcap_authreply *) &sendbuf[sendbufidx];

	if (sock_bufferize(NULL, sizeof(struct rpcap_authreply), NULL, &sendbufidx,
	    RPCAP_NETBUF_SIZE, SOCKBUF_CHECKONLY, errmsgbuf, PCAP_ERRBUF_SIZE) == -1)
		goto error;

	memset(authreply, 0, sizeof(struct rpcap_authreply));
	authreply->minvers = RPCAP_MIN_VERSION;
	authreply->maxvers = RPCAP_MAX_VERSION;

	if (sock_send(pars->sockctrl, sendbuf, sendbufidx, errbuf, PCAP_ERRBUF_SIZE) == -1)
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
	if (rpcap_senderror(pars->sockctrl, 0, PCAP_ERR_AUTH, errmsgbuf,
	    errbuf) == -1)
	{
		rpcapd_log(LOGPRIO_ERROR, "Send to client failed: %s", errbuf);
		return -1;
	}

error_noreply:
	if (rpcapd_discard(pars->sockctrl, plen) == -1)
	{
		return -1;
	}

	return -2;
}
