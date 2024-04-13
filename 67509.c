check_firmware_version(YK_KEY *yk, bool verbose, bool quiet, FILE *debug_file)
{
	YK_STATUS *st = ykds_alloc();

	if (!yk_get_status(yk, st)) {
		free(st);
		return 0;
	}

	if (verbose) {
		D(debug_file, "YubiKey Firmware version: %d.%d.%d\n",
		       ykds_version_major(st),
		       ykds_version_minor(st),
		       ykds_version_build(st));
	}

	if (ykds_version_major(st) < 2 ||
	    (ykds_version_major(st) == 2
         && ykds_version_minor(st) < 2)) {
		if (! quiet)
			fprintf(stderr, "Challenge-response not supported before YubiKey 2.2.\n");
		free(st);
		return 0;
	}

	free(st);
	return 1;
}
