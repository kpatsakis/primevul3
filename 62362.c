nts_to_str(int nts)
{
	switch(nts)
	{
	case NTS_SSDP_ALIVE:
		return "ssdp:alive";
	case NTS_SSDP_BYEBYE:
		return "ssdp:byebye";
	case NTS_SSDP_UPDATE:
		return "ssdp:update";
	}
	return "unknown";
}
