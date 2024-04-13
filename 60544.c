need_numeric_port_hack(void)
{
	if (!tested_for_getaddrinfo_hacks)
		test_for_getaddrinfo_hacks();
	return need_numeric_port_hack_;
}
