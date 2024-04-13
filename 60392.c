evdns_config_windows_nameservers(void)
{
	if (!current_base) {
		current_base = evdns_base_new(NULL, 1);
		return current_base == NULL ? -1 : 0;
	} else {
		return evdns_base_config_windows_nameservers(current_base);
	}
}
