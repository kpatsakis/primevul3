static bool handle_client_startup(PgSocket *client, PktHdr *pkt)
{
	const char *passwd;
	const uint8_t *key;
	bool ok;

	SBuf *sbuf = &client->sbuf;

	/* don't tolerate partial packets */
	if (incomplete_pkt(pkt)) {
		disconnect_client(client, true, "client sent partial pkt in startup phase");
		return false;
	}

	if (client->wait_for_welcome) {
		if  (finish_client_login(client)) {
			/* the packet was already parsed */
			sbuf_prepare_skip(sbuf, pkt->len);
			return true;
		} else {
			return false;
		}
	}

	switch (pkt->type) {
	case PKT_SSLREQ:
		slog_noise(client, "C: req SSL");

		if (client->sbuf.tls) {
			disconnect_client(client, false, "SSL req inside SSL");
			return false;
		}
		if (cf_client_tls_sslmode != SSLMODE_DISABLED) {
			slog_noise(client, "P: SSL ack");
			if (!sbuf_answer(&client->sbuf, "S", 1)) {
				disconnect_client(client, false, "failed to ack SSL");
				return false;
			}
			if (!sbuf_tls_accept(&client->sbuf)) {
				disconnect_client(client, false, "failed to accept SSL");
				return false;
			}
			break;
		}

		/* reject SSL attempt */
		slog_noise(client, "P: nak");
		if (!sbuf_answer(&client->sbuf, "N", 1)) {
			disconnect_client(client, false, "failed to nak SSL");
			return false;
		}
		break;
	case PKT_STARTUP_V2:
		disconnect_client(client, true, "Old V2 protocol not supported");
		return false;
	case PKT_STARTUP:
		/* require SSL except on unix socket */
		if (cf_client_tls_sslmode >= SSLMODE_REQUIRE && !client->sbuf.tls && !pga_is_unix(&client->remote_addr)) {
			disconnect_client(client, true, "SSL required");
			return false;
		}

		if (client->pool && !client->wait_for_user_conn && !client->wait_for_user) {
			disconnect_client(client, true, "client re-sent startup pkt");
			return false;
		}

		if (client->wait_for_user) {
			client->wait_for_user = false;
			if (!finish_set_pool(client, false))
				return false;
		} else if (!decide_startup_pool(client, pkt)) {
			return false;
		}

		break;
	case 'p':		/* PasswordMessage */
		/* too early */
		if (!client->auth_user) {
			disconnect_client(client, true, "client password pkt before startup packet");
			return false;
		}

		ok = mbuf_get_string(&pkt->data, &passwd);
		if (ok && check_client_passwd(client, passwd)) {
			if (!finish_client_login(client))
				return false;
		} else {
			disconnect_client(client, true, "Auth failed");
			return false;
		}
		break;
	case PKT_CANCEL:
		if (mbuf_avail_for_read(&pkt->data) == BACKENDKEY_LEN
		    && mbuf_get_bytes(&pkt->data, BACKENDKEY_LEN, &key))
		{
			memcpy(client->cancel_key, key, BACKENDKEY_LEN);
			accept_cancel_request(client);
		} else {
			disconnect_client(client, false, "bad cancel request");
		}
		return false;
	default:
		disconnect_client(client, false, "bad packet");
		return false;
	}
	sbuf_prepare_skip(sbuf, pkt->len);
	client->request_time = get_cached_time();
	return true;
}
