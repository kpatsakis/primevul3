int main(int argc, char *argv[])
{
	int i, c, sd;
	int log_level = LOG_NOTICE;
	int log_opts = LOG_CONS | LOG_PID;
	int interval = NOTIFY_INTERVAL;
	int refresh = REFRESH_INTERVAL;
	time_t now, rtmo = 0, itmo = 0;

	while ((c = getopt(argc, argv, "dhi:r:v")) != EOF) {
		switch (c) {
		case 'd':
			debug = 1;
			break;

		case 'h':
			return usage(0);

		case 'i':
			interval = atoi(optarg);
			if (interval < 30 || interval > 900)
				errx(1, "Invalid announcement interval (30-900).");
			break;

		case 'r':
			refresh = atoi(optarg);
			if (refresh < 5 || refresh > 1800)
				errx(1, "Invalid refresh interval (5-1800).");
			break;

		case 'v':
			puts(PACKAGE_VERSION);
			return 0;

		default:
			break;
		}
	}

	signal_init();

        if (debug) {
		log_level = LOG_DEBUG;
                log_opts |= LOG_PERROR;
	}

        openlog(PACKAGE_NAME, log_opts, LOG_DAEMON);
        setlogmask(LOG_UPTO(log_level));

	uuidgen();
	lsb_init();
	web_init();

	sd = multicast_init();
	if (sd < 0)
		err(1, "Failed creating multicast socket");

	while (running) {
		now = time(NULL);

		if (rtmo <= now) {
			if (ssdp_init(sd, &argv[optind], argc - optind) > 0)
				announce(1);
			rtmo = now + refresh;
		}

		if (itmo <= now) {
			announce(0);
			itmo = now + interval;
		}

		wait_message(MIN(rtmo, itmo));
	}

	closelog();
	return close_socket();
}
