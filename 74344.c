static void bin_elf_versioninfo(RCore *r, int mode) {
	const char *format = "bin/cur/info/versioninfo/%s%d";
	int num_versym;
	int num_verneed = 0;
	int num_version = 0;
	Sdb *sdb = NULL;
	const char *oValue = NULL;
	bool firstit_for_versym = true;
	if (IS_MODE_JSON (mode)) {
		r_cons_printf ("{\"versym\":[");
	}
	for (num_versym = 0;; num_versym++) {
		const char *versym_path = sdb_fmt (format, "versym", num_versym);
		if (!(sdb = sdb_ns_path (r->sdb, versym_path, 0))) {
			break;
		}
		ut64 addr = sdb_num_get (sdb, "addr", 0);
		ut64 offset = sdb_num_get (sdb, "offset", 0);
		ut64 link = sdb_num_get (sdb, "link", 0);
		ut64 num_entries = sdb_num_get (sdb, "num_entries", 0);
		const char *section_name = sdb_const_get (sdb, "section_name", 0);
		const char *link_section_name = sdb_const_get (sdb, "link_section_name", 0);

		if (IS_MODE_JSON (mode)) {
			if (!firstit_for_versym) { r_cons_printf (","); }
			r_cons_printf ("{\"section_name\":\"%s\",\"address\":%"PFMT64u",\"offset\":%"PFMT64u",",
					section_name, (ut64)addr, (ut64)offset);
			r_cons_printf ("\"link\":%"PFMT64u",\"link_section_name\":\"%s\",\"entries\":[",
					(ut32)link, link_section_name);
		} else {
			r_cons_printf ("Version symbols section '%s' contains %"PFMT64u" entries:\n", section_name, num_entries);
			r_cons_printf (" Addr: 0x%08"PFMT64x"  Offset: 0x%08"PFMT64x"  Link: %x (%s)\n",
				(ut64)addr, (ut64)offset, (ut32)link, link_section_name);
		}
		int i;
		for (i = 0; i < num_entries; i++) {
			const char *key = sdb_fmt ("entry%d", i);
			const char *value = sdb_const_get (sdb, key, 0);
			if (value) {
				if (oValue && !strcmp (value, oValue)) {
					continue;
				}
				if (IS_MODE_JSON (mode)) {
					if (i > 0) { r_cons_printf (","); }
					char *escaped_value = r_str_escape (value);
					r_cons_printf ("{\"idx\":%"PFMT64u",\"value\":\"%s\"}",
							(ut64) i, escaped_value);
					free (escaped_value);
				} else {
					r_cons_printf ("  0x%08"PFMT64x": ", (ut64) i);
					r_cons_printf ("%s\n", value);
				}
				oValue = value;
			}
		}
		if (IS_MODE_JSON (mode)) {
			r_cons_printf ("]}");
		} else {
			r_cons_printf ("\n\n");
		}
		firstit_for_versym = false;
	}
	if (IS_MODE_JSON (mode)) {
		r_cons_printf ("],\"verneed\":[");
	}

	bool firstit_dowhile_verneed = true;
	do {
		char *verneed_path = r_str_newf (format, "verneed", num_verneed++);
		if (!(sdb = sdb_ns_path (r->sdb, verneed_path, 0))) {
			break;
		}
		if (IS_MODE_JSON (mode)) {
			if (!firstit_dowhile_verneed) {
				r_cons_printf (",");
			}
			r_cons_printf ("{\"section_name\":\"%s\",\"address\":%"PFMT64u",\"offset\":%"PFMT64u",",
				sdb_const_get (sdb, "section_name", 0), sdb_num_get (sdb, "addr", 0), sdb_num_get (sdb, "offset", 0));
			r_cons_printf ("\"link\":%"PFMT64u",\"link_section_name\":\"%s\",\"entries\":[",
				sdb_num_get (sdb, "link", 0), sdb_const_get (sdb, "link_section_name", 0));
		} else {
			r_cons_printf ("Version need section '%s' contains %d entries:\n",
				sdb_const_get (sdb, "section_name", 0), (int)sdb_num_get (sdb, "num_entries", 0));

			r_cons_printf (" Addr: 0x%08"PFMT64x, sdb_num_get (sdb, "addr", 0));

			r_cons_printf ("  Offset: 0x%08"PFMT64x"  Link to section: %"PFMT64d" (%s)\n",
				sdb_num_get (sdb, "offset", 0), sdb_num_get (sdb, "link", 0),
				sdb_const_get (sdb, "link_section_name", 0));
		}
		bool firstit_for_verneed = true;
		for (num_version = 0;; num_version++) {
			const char *filename = NULL;
			int num_vernaux = 0;

			char *path_version = sdb_fmt ("%s/version%d", verneed_path, num_version);
			if (!(sdb = sdb_ns_path (r->sdb, path_version, 0))) {
				break;
			}
			if (IS_MODE_JSON (mode)) {
				if (!firstit_for_verneed) { r_cons_printf (","); }
				r_cons_printf ("{\"idx\":%"PFMT64u",\"vn_version\":%d,",
					sdb_num_get (sdb, "idx", 0), (int)sdb_num_get (sdb, "vn_version", 0));
			} else {
				r_cons_printf ("  0x%08"PFMT64x": Version: %d",
					sdb_num_get (sdb, "idx", 0), (int)sdb_num_get (sdb, "vn_version", 0));
			}

			if ((filename = sdb_const_get (sdb, "file_name", 0))) {
				if (IS_MODE_JSON (mode)) {
					char *escaped_filename = r_str_escape (filename);
					r_cons_printf ("\"file_name\":\"%s\",", escaped_filename);
					free (escaped_filename);
				} else {
					r_cons_printf ("  File: %s", filename);
				}
			}
			if (IS_MODE_JSON (mode)) {
				r_cons_printf ("\"cnt\":%d,", (int)sdb_num_get (sdb, "cnt", 0));
			} else {
				r_cons_printf ("  Cnt: %d\n", (int)sdb_num_get (sdb, "cnt", 0));
			}
			if (IS_MODE_JSON (mode)) {
				r_cons_printf ("\"vernaux\":[");
			}
			bool firstit_dowhile_vernaux = true;
			do {
				const char *path_vernaux = sdb_fmt ("%s/vernaux%d", path_version, num_vernaux++);
				if (!(sdb = sdb_ns_path (r->sdb, path_vernaux, 0))) {
					break;
				}
				if (IS_MODE_JSON (mode)) {
					if (!firstit_dowhile_vernaux) { r_cons_printf (","); }
					r_cons_printf ("{\"idx\":%"PFMT64u",\"name\":\"%s\",",
						sdb_num_get (sdb, "idx", 0), sdb_const_get (sdb, "name", 0));
					r_cons_printf ("\"flags\":\"%s\",\"version\":%d}",
						sdb_const_get (sdb, "flags", 0), (int)sdb_num_get (sdb, "version", 0));
				} else {
					r_cons_printf ("  0x%08"PFMT64x":   Name: %s",
						sdb_num_get (sdb, "idx", 0), sdb_const_get (sdb, "name", 0));
					r_cons_printf ("  Flags: %s Version: %d\n",
						sdb_const_get (sdb, "flags", 0), (int)sdb_num_get (sdb, "version", 0));
				}
				firstit_dowhile_vernaux = false;
			} while (sdb);
			if (IS_MODE_JSON (mode)) {
				r_cons_printf ("]}");
			}
			firstit_for_verneed = false;
		}
		if (IS_MODE_JSON (mode)) {
			r_cons_printf ("]}");
		}
		firstit_dowhile_verneed = false;
		free (verneed_path);
	} while (sdb);
	if (IS_MODE_JSON (mode)) {
		r_cons_printf ("]}");
	}
}
