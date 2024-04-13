evdns_init(void)
{
	struct evdns_base *base = evdns_base_new(NULL, 1);
	if (base) {
		current_base = base;
		return 0;
	} else {
		return -1;
	}
}
