proto_itoa(int proto)
{
	const char * protocol;
	switch(proto) {
	case IPPROTO_UDP:
		protocol = "UDP";
		break;
	case IPPROTO_TCP:
		protocol = "TCP";
		break;
#ifdef IPPROTO_UDPLITE
	case IPPROTO_UDPLITE:
		protocol = "UDPLITE";
		break;
#endif /* IPPROTO_UDPLITE */
	default:
		protocol = "*UNKNOWN*";
	}
	return protocol;
}
