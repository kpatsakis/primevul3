int sasl_capability_visible(void)
{
	if (!SASL_SERVER || !find_server(SASL_SERVER, NULL))
		return 0;
	
	return 1;
}
