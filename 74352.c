static void bin_pe_resources(RCore *r, int mode) {
	Sdb *sdb = NULL;
	int index = 0;
	PJ *pj = NULL;
	const char *pe_path = "bin/cur/info/pe_resource";
	if (!(sdb = sdb_ns_path (r->sdb, pe_path, 0))) {
		return;
	}
	if (IS_MODE_SET (mode)) {
		r_flag_space_set (r->flags, R_FLAGS_FS_RESOURCES);
	} else if (IS_MODE_RAD (mode)) {
		r_cons_printf ("fs resources\n");
	} else if (IS_MODE_JSON (mode)) {
		pj = pj_new ();
		pj_a (pj);
	}
	while (true) {
		const char *timestrKey = sdb_fmt ("resource.%d.timestr", index);
		const char *vaddrKey = sdb_fmt ("resource.%d.vaddr", index);
		const char *sizeKey  = sdb_fmt ("resource.%d.size", index);
		const char *typeKey  = sdb_fmt ("resource.%d.type", index);
		const char *languageKey = sdb_fmt ("resource.%d.language", index);
		const char *nameKey = sdb_fmt ("resource.%d.name", index);
		char *timestr = sdb_get (sdb, timestrKey, 0);
		if (!timestr) {
			break;
		}
		ut64 vaddr = sdb_num_get (sdb, vaddrKey, 0);
		int size = (int)sdb_num_get (sdb, sizeKey, 0);
		char *name = sdb_get (sdb, nameKey, 0);
		char *type = sdb_get (sdb, typeKey, 0);
		char *lang = sdb_get (sdb, languageKey, 0);

		if (IS_MODE_SET (mode)) {
			const char *name = sdb_fmt ("resource.%d", index);
			r_flag_set (r->flags, name, vaddr, size);
		} else if (IS_MODE_RAD (mode)) {
			r_cons_printf ("f resource.%d %d 0x%08"PFMT32x"\n", index, size, vaddr);
		} else if (IS_MODE_JSON (mode)) {
			pj_o (pj);
			pj_ks (pj, "name", name);
			pj_ki (pj, "index", index);
			pj_ks (pj, "type", type);
			pj_kn (pj, "vaddr", vaddr);
			pj_ki (pj, "size", size);
			pj_ks (pj, "lang", lang);
			pj_ks (pj, "timestamp", timestr);
			pj_end (pj);
		} else {
			char humansz[8];
			r_num_units (humansz, sizeof (humansz), size);
			r_cons_printf ("Resource %d\n", index);
			r_cons_printf ("  name: %s\n", name);
			r_cons_printf ("  timestamp: %s\n", timestr);
			r_cons_printf ("  vaddr: 0x%08"PFMT64x"\n", vaddr);
			r_cons_printf ("  size: %s\n", humansz);
			r_cons_printf ("  type: %s\n", type);
			r_cons_printf ("  language: %s\n", lang);
		}

		R_FREE (timestr);
		R_FREE (name);
		R_FREE (type);
		R_FREE (lang)

		index++;
	}
	if (IS_MODE_JSON (mode)) {
		pj_end (pj);
		r_cons_printf ("%s\n", pj_string (pj));
		pj_free (pj);
	} else if (IS_MODE_RAD (mode)) {
		r_cons_printf ("fs *");
	}
}
