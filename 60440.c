evdns_set_option(const char *option, const char *val, int flags)
{
	if (!current_base)
		current_base = evdns_base_new(NULL, 0);
	return evdns_base_set_option(current_base, option, val);
}
