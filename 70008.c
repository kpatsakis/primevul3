static bool ipip6_valid_ip_proto(u8 ipproto)
{
	return ipproto == IPPROTO_IPV6 ||
		ipproto == IPPROTO_IPIP ||
#if IS_ENABLED(CONFIG_MPLS)
		ipproto == IPPROTO_MPLS ||
#endif
		ipproto == 0;
}
