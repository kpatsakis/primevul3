static void bin_pe_versioninfo(RCore *r, int mode) {
	Sdb *sdb = NULL;
	int num_version = 0;
	int num_stringtable = 0;
	int num_string = 0;
	const char *format_version = "bin/cur/info/vs_version_info/VS_VERSIONINFO%d";
	const char *format_stringtable = "%s/string_file_info/stringtable%d";
	const char *format_string = "%s/string%d";
	if (!IS_MODE_JSON (mode)) {
		r_cons_printf ("=== VS_VERSIONINFO ===\n\n");
	}
	bool firstit_dowhile = true;
	do {
		char *path_version = sdb_fmt (format_version, num_version);
		if (!(sdb = sdb_ns_path (r->sdb, path_version, 0))) {
			break;
		}
		if (!firstit_dowhile && IS_MODE_JSON (mode)) {
			r_cons_printf (",");
		}
		if (IS_MODE_JSON (mode)) {
			r_cons_printf ("{\"VS_FIXEDFILEINFO\":{");
		} else {
			r_cons_printf ("# VS_FIXEDFILEINFO\n\n");
		}
		const char *path_fixedfileinfo = sdb_fmt ("%s/fixed_file_info", path_version);
		if (!(sdb = sdb_ns_path (r->sdb, path_fixedfileinfo, 0))) {
			r_cons_printf ("}");
			break;
		}
		if (IS_MODE_JSON (mode)) {
			r_cons_printf ("\"Signature\":%"PFMT64u",", sdb_num_get (sdb, "Signature", 0));
			r_cons_printf ("\"StrucVersion\":%"PFMT64u",", sdb_num_get (sdb, "StrucVersion", 0));
			r_cons_printf ("\"FileVersion\":\"%"PFMT64d".%"PFMT64d".%"PFMT64d".%"PFMT64d"\",",
				sdb_num_get (sdb, "FileVersionMS", 0) >> 16,
				sdb_num_get (sdb, "FileVersionMS", 0) & 0xFFFF,
				sdb_num_get (sdb, "FileVersionLS", 0) >> 16,
				sdb_num_get (sdb, "FileVersionLS", 0) & 0xFFFF);
			r_cons_printf ("\"ProductVersion\":\"%"PFMT64d".%"PFMT64d".%"PFMT64d".%"PFMT64d"\",",
				sdb_num_get (sdb, "ProductVersionMS", 0) >> 16,
				sdb_num_get (sdb, "ProductVersionMS", 0) & 0xFFFF,
				sdb_num_get (sdb, "ProductVersionLS", 0) >> 16,
				sdb_num_get (sdb, "ProductVersionLS", 0) & 0xFFFF);
			r_cons_printf ("\"FileFlagsMask\":%"PFMT64u",", sdb_num_get (sdb, "FileFlagsMask", 0));
			r_cons_printf ("\"FileFlags\":%"PFMT64u",", sdb_num_get (sdb, "FileFlags", 0));
			r_cons_printf ("\"FileOS\":%"PFMT64u",", sdb_num_get (sdb, "FileOS", 0));
			r_cons_printf ("\"FileType\":%"PFMT64u",", sdb_num_get (sdb, "FileType", 0));
			r_cons_printf ("\"FileSubType\":%"PFMT64u, sdb_num_get (sdb, "FileSubType", 0));
			r_cons_printf ("},");
		} else {
			r_cons_printf ("  Signature: 0x%"PFMT64x"\n", sdb_num_get (sdb, "Signature", 0));
			r_cons_printf ("  StrucVersion: 0x%"PFMT64x"\n", sdb_num_get (sdb, "StrucVersion", 0));
			r_cons_printf ("  FileVersion: %"PFMT64d".%"PFMT64d".%"PFMT64d".%"PFMT64d"\n",
				sdb_num_get (sdb, "FileVersionMS", 0) >> 16,
				sdb_num_get (sdb, "FileVersionMS", 0) & 0xFFFF,
				sdb_num_get (sdb, "FileVersionLS", 0) >> 16,
				sdb_num_get (sdb, "FileVersionLS", 0) & 0xFFFF);
			r_cons_printf ("  ProductVersion: %"PFMT64d".%"PFMT64d".%"PFMT64d".%"PFMT64d"\n",
				sdb_num_get (sdb, "ProductVersionMS", 0) >> 16,
				sdb_num_get (sdb, "ProductVersionMS", 0) & 0xFFFF,
				sdb_num_get (sdb, "ProductVersionLS", 0) >> 16,
				sdb_num_get (sdb, "ProductVersionLS", 0) & 0xFFFF);
			r_cons_printf ("  FileFlagsMask: 0x%"PFMT64x"\n", sdb_num_get (sdb, "FileFlagsMask", 0));
			r_cons_printf ("  FileFlags: 0x%"PFMT64x"\n", sdb_num_get (sdb, "FileFlags", 0));
			r_cons_printf ("  FileOS: 0x%"PFMT64x"\n", sdb_num_get (sdb, "FileOS", 0));
			r_cons_printf ("  FileType: 0x%"PFMT64x"\n", sdb_num_get (sdb, "FileType", 0));
			r_cons_printf ("  FileSubType: 0x%"PFMT64x"\n", sdb_num_get (sdb, "FileSubType", 0));
			r_cons_newline ();
		}
#if 0
		r_cons_printf ("  FileDate: %d.%d.%d.%d\n",
			sdb_num_get (sdb, "FileDateMS", 0) >> 16,
			sdb_num_get (sdb, "FileDateMS", 0) & 0xFFFF,
			sdb_num_get (sdb, "FileDateLS", 0) >> 16,
			sdb_num_get (sdb, "FileDateLS", 0) & 0xFFFF);
#endif
		if (IS_MODE_JSON (mode)) {
			r_cons_printf ("\"StringTable\":{");
		} else {
			r_cons_printf ("# StringTable\n\n");
		}
		for (num_stringtable = 0; sdb; num_stringtable++) {
			char *path_stringtable = r_str_newf (format_stringtable, path_version, num_stringtable);
			sdb = sdb_ns_path (r->sdb, path_stringtable, 0);
			bool firstit_for = true;
			for (num_string = 0; sdb; num_string++) {
				char *path_string = r_str_newf (format_string, path_stringtable, num_string);
				sdb = sdb_ns_path (r->sdb, path_string, 0);
				if (sdb) {
					if (!firstit_for && IS_MODE_JSON (mode)) { r_cons_printf (","); }
					int lenkey = 0;
					int lenval = 0;
					ut8 *key_utf16 = sdb_decode (sdb_const_get (sdb, "key", 0), &lenkey);
					ut8 *val_utf16 = sdb_decode (sdb_const_get (sdb, "value", 0), &lenval);
					ut8 *key_utf8 = calloc (lenkey * 2, 1);
					ut8 *val_utf8 = calloc (lenval * 2, 1);
					if (r_str_utf16_to_utf8 (key_utf8, lenkey * 2, key_utf16, lenkey, true) < 0
						|| r_str_utf16_to_utf8 (val_utf8, lenval * 2, val_utf16, lenval, true) < 0) {
						eprintf ("Warning: Cannot decode utf16 to utf8\n");
					} else if (IS_MODE_JSON (mode)) {
						char *escaped_key_utf8 = r_str_escape ((char*)key_utf8);
						char *escaped_val_utf8 = r_str_escape ((char*)val_utf8);
						r_cons_printf ("\"%s\":\"%s\"", escaped_key_utf8, escaped_val_utf8);
						free (escaped_key_utf8);
						free (escaped_val_utf8);
					} else {
						r_cons_printf ("  %s: %s\n", (char*)key_utf8, (char*)val_utf8);
					}
					free (key_utf8);
					free (val_utf8);
					free (key_utf16);
					free (val_utf16);
				}
				firstit_for = false;
				free (path_string);
			}
			free (path_stringtable);
		}
		if (IS_MODE_JSON (mode)) {
			r_cons_printf ("}}");
		}
		num_version++;
		firstit_dowhile = false;
	} while (sdb);
