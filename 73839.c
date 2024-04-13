int ssl3_send_alert(SSL *s, int level, int desc)
	{
	/* Map tls/ssl alert value to correct one */
	desc=s->method->ssl3_enc->alert_value(desc);
	if (s->version == SSL3_VERSION && desc == SSL_AD_PROTOCOL_VERSION)
		desc = SSL_AD_HANDSHAKE_FAILURE; /* SSL 3.0 does not have protocol_version alerts */
	if (desc < 0) return -1;
	/* If a fatal one, remove from cache */
	if ((level == SSL3_AL_FATAL) && (s->session != NULL))
		SSL_CTX_remove_session(s->ctx,s->session);

	s->s3->alert_dispatch=1;
	s->s3->send_alert[0]=level;
	s->s3->send_alert[1]=desc;
	if (s->s3->wbuf.left == 0) /* data still being written out? */
		return s->method->ssl_dispatch_alert(s);
	/* else data is still being written out, we will get written
	 * some time in the future */
	return -1;
	}
