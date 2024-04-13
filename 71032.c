GetAssignedRoles(struct upnphttp * h, const char * action, const char * ns)
{
	static const char resp[] =
		"<u:%sResponse "
		"xmlns:u=\"%s\">"
		"<RoleList>%s</RoleList>"
		"</u:%sResponse>";
	char body[1024];
	int bodylen;
	const char * RoleList = "Public"; /* list of roles separated by spaces */

#ifdef ENABLE_HTTPS
	if(h->ssl != NULL) {
		/* we should get the Roles of the session (based on client certificate) */
		X509 * peercert;
		peercert = SSL_get_peer_certificate(h->ssl);
		if(peercert != NULL) {
			RoleList = "Admin Basic";
			X509_free(peercert);
		}
	}
#endif

	bodylen = snprintf(body, sizeof(body), resp,
	                   action, ns/*"urn:schemas-upnp-org:service:DeviceProtection:1"*/,
	                   RoleList, action);
	BuildSendAndCloseSoapResp(h, body, bodylen);
}
