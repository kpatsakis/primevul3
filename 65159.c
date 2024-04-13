input_gssapi_mic(int type, u_int32_t plen, struct ssh *ssh)
{
	Authctxt *authctxt = ssh->authctxt;
	Gssctxt *gssctxt;
	int r, authenticated = 0;
	struct sshbuf *b;
	gss_buffer_desc mic, gssbuf;
	const char *displayname;
	u_char *p;
	size_t len;

	if (authctxt == NULL || (authctxt->methoddata == NULL && !use_privsep))
		fatal("No authentication or GSSAPI context");

	gssctxt = authctxt->methoddata;

	if ((r = sshpkt_get_string(ssh, &p, &len)) != 0)
		fatal("%s: %s", __func__, ssh_err(r));
	if ((b = sshbuf_new()) == NULL)
		fatal("%s: sshbuf_new failed", __func__);
	mic.value = p;
	mic.length = len;
	ssh_gssapi_buildmic(b, authctxt->user, authctxt->service,
	    "gssapi-with-mic");

	if ((gssbuf.value = sshbuf_mutable_ptr(b)) == NULL)
		fatal("%s: sshbuf_mutable_ptr failed", __func__);
	gssbuf.length = sshbuf_len(b);

	if (!GSS_ERROR(PRIVSEP(ssh_gssapi_checkmic(gssctxt, &gssbuf, &mic))))
		authenticated = PRIVSEP(ssh_gssapi_userok(authctxt->user));
	else
		logit("GSSAPI MIC check failed");

	sshbuf_free(b);
	free(mic.value);

	if ((!use_privsep || mm_is_monitor()) &&
	    (displayname = ssh_gssapi_displayname()) != NULL)
		auth2_record_info(authctxt, "%s", displayname);

	authctxt->postponed = 0;
	ssh_dispatch_set(ssh, SSH2_MSG_USERAUTH_GSSAPI_TOKEN, NULL);
	ssh_dispatch_set(ssh, SSH2_MSG_USERAUTH_GSSAPI_ERRTOK, NULL);
	ssh_dispatch_set(ssh, SSH2_MSG_USERAUTH_GSSAPI_MIC, NULL);
	ssh_dispatch_set(ssh, SSH2_MSG_USERAUTH_GSSAPI_EXCHANGE_COMPLETE, NULL);
	userauth_finish(ssh, authenticated, "gssapi-with-mic", NULL);
	return 0;
}
