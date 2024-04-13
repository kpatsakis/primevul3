static int bin_imports(RCore *r, int mode, int va, const char *name) {
	RBinInfo *info = r_bin_get_info (r->bin);
	int bin_demangle = r_config_get_i (r->config, "bin.demangle");
	bool keep_lib = r_config_get_i (r->config, "bin.demangle.libs");
	RBinImport *import;
	RListIter *iter;
	bool lit = info ? info->has_lit: false;
	char *str;
	int i = 0;

	if (!info) {
		return false;
	}

	RList *imports = r_bin_get_imports (r->bin);
	int cdsz = info? (info->bits == 64? 8: info->bits == 32? 4: info->bits == 16 ? 4: 0): 0;
	if (IS_MODE_JSON (mode)) {
		r_cons_print ("[");
	} else if (IS_MODE_RAD (mode)) {
		r_cons_println ("fs imports");
	} else if (IS_MODE_NORMAL (mode)) {
		r_cons_println ("[Imports]");
		r_cons_println ("Num  Vaddr       Bind      Type Name");
	}
	r_list_foreach (imports, iter, import) {
		if (name && strcmp (import->name, name)) {
			continue;
		}
		char *symname = strdup (import->name);
		ut64 addr = lit ? r_core_bin_impaddr (r->bin, va, symname): 0;
		if (bin_demangle) {
			char *dname = r_bin_demangle (r->bin->cur, NULL, symname, addr, keep_lib);
			if (dname) {
				free (symname);
				symname = r_str_newf ("sym.imp.%s", dname);
				free (dname);
			}
		}
		if (r->bin->prefix) {
			char *prname = r_str_newf ("%s.%s", r->bin->prefix, symname);
			free (symname);
			symname = prname;
		}
		if (IS_MODE_SET (mode)) {
			if (strstr (symname, ".dll_") && cdsz) {
				r_meta_add (r->anal, R_META_TYPE_DATA, addr, addr + cdsz, NULL);
			}
		} else if (IS_MODE_SIMPLE (mode) || IS_MODE_SIMPLEST (mode)) {
			r_cons_println (symname);
		} else if (IS_MODE_JSON (mode)) {
			str = r_str_escape_utf8_for_json (symname, -1);
			str = r_str_replace (str, "\"", "\\\"", 1);
			r_cons_printf ("%s{\"ordinal\":%d,"
				"\"bind\":\"%s\","
				"\"type\":\"%s\",",
				iter->p ? "," : "",
				import->ordinal,
				import->bind,
				import->type);
			if (import->classname && import->classname[0]) {
				r_cons_printf ("\"classname\":\"%s\","
					"\"descriptor\":\"%s\",",
					import->classname,
					import->descriptor);
			}
			r_cons_printf ("\"name\":\"%s\",\"plt\":%"PFMT64d"}",
				str, addr);
			free (str);
		} else if (IS_MODE_RAD (mode)) {
		} else {
			const char *bind = r_str_get (import->bind);
			const char *type = r_str_get (import->type);
#if 0
			r_cons_printf ("ordinal=%03d plt=0x%08"PFMT64x" bind=%s type=%s",
				import->ordinal, addr, bind, type);
			if (import->classname && import->classname[0]) {
				r_cons_printf (" classname=%s", import->classname);
			}
			r_cons_printf (" name=%s", symname);
			if (import->descriptor && import->descriptor[0]) {
				r_cons_printf (" descriptor=%s", import->descriptor);
			}
			r_cons_newline ();
#else
			r_cons_printf ("%4d 0x%08"PFMT64x" %7s %7s ",
				import->ordinal, addr, bind, type);
			if (import->classname && import->classname[0]) {
				r_cons_printf ("%s.", import->classname);
			}
			r_cons_printf ("%s", symname);
			if (import->descriptor && import->descriptor[0]) {
				r_cons_printf (" descriptor=%s", import->descriptor);
			}
			r_cons_newline ();
#endif
		}
		R_FREE (symname);
		i++;
	}
	if (IS_MODE_JSON (mode)) {
		r_cons_print ("]");
	} else if (IS_MODE_NORMAL (mode)) {
	}
#if MYDB
	osymbols = NULL;
	sdb_free (mydb);
	mydb = NULL;
#endif
	return true;
}
