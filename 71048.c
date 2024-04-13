SendSetupMessage(struct upnphttp * h, const char * action, const char * ns)
{
	static const char resp[] =
		"<u:%sResponse "
		"xmlns:u=\"%s\">"
		"<OutMessage>%s</OutMessage>"
		"</u:%sResponse>";
	char body[1024];
	int bodylen;
	struct NameValueParserData data;
	const char * ProtocolType;	/* string */
	const char * InMessage;		/* base64 */
	const char * OutMessage = "";	/* base64 */

	ParseNameValue(h->req_buf + h->req_contentoff, h->req_contentlen, &data);
	ProtocolType = GetValueFromNameValueList(&data, "ProtocolType");	/* string */
	InMessage = GetValueFromNameValueList(&data, "InMessage");	/* base64 */

	if(ProtocolType == NULL || InMessage == NULL)
	{
		ClearNameValueList(&data);
		SoapError(h, 402, "Invalid Args");
		return;
	}
	/*if(strcmp(ProtocolType, "DeviceProtection:1") != 0)*/
	if(strcmp(ProtocolType, "WPS") != 0)
	{
		ClearNameValueList(&data);
		SoapError(h, 600, "Argument Value Invalid"); /* 703 ? */
		return;
	}
	/* TODO : put here code for WPS */

	bodylen = snprintf(body, sizeof(body), resp,
	                   action, ns/*"urn:schemas-upnp-org:service:DeviceProtection:1"*/,
	                   OutMessage, action);
	BuildSendAndCloseSoapResp(h, body, bodylen);
	ClearNameValueList(&data);
}
