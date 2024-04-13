evdns_base_config_windows_nameservers(struct evdns_base *base)
{
	int r;
	char *fname;
	if (base == NULL)
		base = current_base;
	if (base == NULL)
		return -1;
	EVDNS_LOCK(base);
	fname = evdns_get_default_hosts_filename();
	log(EVDNS_LOG_DEBUG, "Loading hosts entries from %s", fname);
	evdns_base_load_hosts(base, fname);
	if (fname)
		mm_free(fname);

	if (load_nameservers_with_getnetworkparams(base) == 0) {
		EVDNS_UNLOCK(base);
		return 0;
	}
	r = load_nameservers_from_registry(base);

	EVDNS_UNLOCK(base);
	return r;
}
