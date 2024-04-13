static void announce(int mod)
{
	struct ifsock *ifs;

	logit(LOG_INFO, "Sending SSDP NOTIFY new:%d ...", mod);

	LIST_FOREACH(ifs, &il, link) {
		size_t i;

		if (mod && !ifs->mod)
			continue;
		ifs->mod = 0;

		for (i = 0; supported_types[i]; i++) {
			/* UUID sent in SSDP_ST_ALL, first announce */
			if (!strcmp(supported_types[i], uuid))
				continue;

			send_message(ifs, supported_types[i], NULL);
		}
	}
}
