evutil_socket_error_to_string(int errcode)
{
	struct cached_sock_errs_entry *errs, *newerr, find;
	char *msg = NULL;

	EVLOCK_LOCK(windows_socket_errors_lock_, 0);

	find.code = errcode;
	errs = HT_FIND(cached_sock_errs_map, &windows_socket_errors, &find);
	if (errs) {
		msg = errs->msg;
		goto done;
	}

	if (0 != FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM |
			       FORMAT_MESSAGE_IGNORE_INSERTS |
			       FORMAT_MESSAGE_ALLOCATE_BUFFER,
			       NULL, errcode, 0, (char *)&msg, 0, NULL))
		chomp (msg);	/* because message has trailing newline */
	else {
		size_t len = 50;
		/* use LocalAlloc because FormatMessage does */
		msg = LocalAlloc(LMEM_FIXED, len);
		if (!msg) {
			msg = (char *)"LocalAlloc failed during Winsock error";
			goto done;
		}
		evutil_snprintf(msg, len, "winsock error 0x%08x", errcode);
	}

	newerr = (struct cached_sock_errs_entry *)
		mm_malloc(sizeof (struct cached_sock_errs_entry));

	if (!newerr) {
		LocalFree(msg);
		msg = (char *)"malloc failed during Winsock error";
		goto done;
	}

	newerr->code = errcode;
	newerr->msg = msg;
	HT_INSERT(cached_sock_errs_map, &windows_socket_errors, newerr);

 done:
	EVLOCK_UNLOCK(windows_socket_errors_lock_, 0);

	return msg;
}
