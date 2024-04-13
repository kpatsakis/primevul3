SetConnectionType(struct upnphttp * h, const char * action, const char * ns)
{
#ifdef UPNP_STRICT
	const char * connection_type;
#endif /* UPNP_STRICT */
	struct NameValueParserData data;
	UNUSED(action);
	UNUSED(ns);

	ParseNameValue(h->req_buf + h->req_contentoff, h->req_contentlen, &data);
#ifdef UPNP_STRICT
	connection_type = GetValueFromNameValueList(&data, "NewConnectionType");
	if(!connection_type) {
		ClearNameValueList(&data);
		SoapError(h, 402, "Invalid Args");
		return;
	}
#endif /* UPNP_STRICT */
	/* Unconfigured, IP_Routed, IP_Bridged */
	ClearNameValueList(&data);
	/* always return a ReadOnly error */
	SoapError(h, 731, "ReadOnly");
}
