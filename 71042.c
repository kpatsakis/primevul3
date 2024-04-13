GetSupportedProtocols(struct upnphttp * h, const char * action, const char * ns)
{
	static const char resp[] =
		"<u:%sResponse "
		"xmlns:u=\"%s\">"
		"<ProtocolList><![CDATA[%s]]></ProtocolList>"
		"</u:%sResponse>";
	char body[1024];
	int bodylen;
	const char * ProtocolList =
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		"<SupportedProtocols xmlns=\"urn:schemas-upnp-org:gw:DeviceProtection\""
		" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\""
		" xsi:schemaLocation=\"urn:schemas-upnp-org:gw:DeviceProtection"
		" http://www.upnp.org/schemas/gw/DeviceProtection-v1.xsd\">"
		"<Introduction><Name>WPS</Name></Introduction>"
		"<Login><Name>PKCS5</Name></Login>"
		"</SupportedProtocols>";

	bodylen = snprintf(body, sizeof(body), resp,
	                   action, ns/*"urn:schemas-upnp-org:service:DeviceProtection:1"*/,
	                   ProtocolList, action);
	BuildSendAndCloseSoapResp(h, body, bodylen);
}
