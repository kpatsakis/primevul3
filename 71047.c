RequestConnection(struct upnphttp * h, const char * action, const char * ns)
{
	UNUSED(action);
	UNUSED(ns);
	SoapError(h, 606, "Action not authorized");
}
