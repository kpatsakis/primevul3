void PrintGPACConfig()
{
	u32 i, j, cfg_count, key_count;
	char szName[200];
	char *secName = NULL;

	fprintf(stderr, "Enter section name (\"*\" for complete dump):\n");
	if (1 > scanf("%s", szName)) {
		fprintf(stderr, "No section name, aborting.\n");
		return;
	}
	if (strcmp(szName, "*")) secName = szName;

	fprintf(stderr, "\n\n*** GPAC Configuration ***\n\n");

	cfg_count = gf_cfg_get_section_count(cfg_file);
	for (i=0; i<cfg_count; i++) {
		const char *sec = gf_cfg_get_section_name(cfg_file, i);
		if (secName) {
			if (stricmp(sec, secName)) continue;
		} else {
			if (!stricmp(sec, "General")) continue;
			if (!stricmp(sec, "MimeTypes")) continue;
			if (!stricmp(sec, "RecentFiles")) continue;
		}
		fprintf(stderr, "[%s]\n", sec);
		key_count = gf_cfg_get_key_count(cfg_file, sec);
		for (j=0; j<key_count; j++) {
			const char *key = gf_cfg_get_key_name(cfg_file, sec, j);
			const char *val = gf_cfg_get_key(cfg_file, sec, key);
			fprintf(stderr, "%s=%s\n", key, val);
		}
		fprintf(stderr, "\n");
	}
}
