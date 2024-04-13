evdns_nameserver_add(unsigned long int address) {
	if (!current_base)
		current_base = evdns_base_new(NULL, 0);
	return evdns_base_nameserver_add(current_base, address);
}
