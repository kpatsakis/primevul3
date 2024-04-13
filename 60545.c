need_socktype_protocol_hack(void)
{
	if (!tested_for_getaddrinfo_hacks)
		test_for_getaddrinfo_hacks();
	return need_socktype_protocol_hack_;
}
