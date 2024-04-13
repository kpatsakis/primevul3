input_gssapi_exchange_complete(int type, u_int32_t plen, struct ssh *ssh)
{
	Authctxt *authctxt = ssh->authctxt;
	int r, authenticated;
	const char *displayname;

	if (authctxt == NULL || (authctxt->methoddata == NULL && !use_privsep))
		fatal("No authentication or GSSAPI context");

	/*
	 * We don't need to check the status, because we're only enabled in
	 * the dispatcher once the exchange is complete
	 */

	if ((r = sshpkt_get_end(ssh)) != 0)
		fatal("%s: %s", __func__, ssh_err(r));

	authenticated = PRIVSEP(ssh_gssapi_userok(authctxt->user));

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
