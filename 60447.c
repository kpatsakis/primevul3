load_nameservers_with_getnetworkparams(struct evdns_base *base)
{
	/* Based on MSDN examples and inspection of  c-ares code. */
	FIXED_INFO *fixed;
	HMODULE handle = 0;
	ULONG size = sizeof(FIXED_INFO);
	void *buf = NULL;
	int status = 0, r, added_any;
	IP_ADDR_STRING *ns;
	GetNetworkParams_fn_t fn;

	ASSERT_LOCKED(base);
	if (!(handle = evutil_load_windows_system_library_(
			TEXT("iphlpapi.dll")))) {
		log(EVDNS_LOG_WARN, "Could not open iphlpapi.dll");
		status = -1;
		goto done;
	}
	if (!(fn = (GetNetworkParams_fn_t) GetProcAddress(handle, "GetNetworkParams"))) {
		log(EVDNS_LOG_WARN, "Could not get address of function.");
		status = -1;
		goto done;
	}

	buf = mm_malloc(size);
	if (!buf) { status = 4; goto done; }
	fixed = buf;
	r = fn(fixed, &size);
	if (r != ERROR_SUCCESS && r != ERROR_BUFFER_OVERFLOW) {
		status = -1;
		goto done;
	}
	if (r != ERROR_SUCCESS) {
		mm_free(buf);
		buf = mm_malloc(size);
		if (!buf) { status = 4; goto done; }
		fixed = buf;
		r = fn(fixed, &size);
		if (r != ERROR_SUCCESS) {
			log(EVDNS_LOG_DEBUG, "fn() failed.");
			status = -1;
			goto done;
		}
	}

	EVUTIL_ASSERT(fixed);
	added_any = 0;
	ns = &(fixed->DnsServerList);
	while (ns) {
		r = evdns_nameserver_ip_add_line(base, ns->IpAddress.String);
		if (r) {
			log(EVDNS_LOG_DEBUG,"Could not add nameserver %s to list,error: %d",
				(ns->IpAddress.String),(int)GetLastError());
			status = r;
		} else {
			++added_any;
			log(EVDNS_LOG_DEBUG,"Successfully added %s as nameserver",ns->IpAddress.String);
		}

		ns = ns->Next;
	}

	if (!added_any) {
		log(EVDNS_LOG_DEBUG, "No nameservers added.");
		if (status == 0)
			status = -1;
	} else {
		status = 0;
	}

 done:
	if (buf)
		mm_free(buf);
	if (handle)
		FreeLibrary(handle);
	return status;
}
