CheckStatus(struct upnphttp * h)
{
	if (GETFLAG(IPV6FCFWDISABLEDMASK))
	{
		SoapError(h, 702, "FirewallDisabled");
		return 0;
	}
	else if(GETFLAG(IPV6FCINBOUNDDISALLOWEDMASK))
	{
		SoapError(h, 703, "InboundPinholeNotAllowed");
		return 0;
	}
	else
		return 1;
}
