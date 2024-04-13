static int bin_entry(RCore *r, int mode, ut64 laddr, int va, bool inifin) {
	char str[R_FLAG_NAME_SIZE];
	RList *entries = r_bin_get_entries (r->bin);
	RListIter *iter;
	RListIter *last_processed = NULL;
	RBinAddr *entry = NULL;
	int i = 0, init_i = 0, fini_i = 0, preinit_i = 0;
	ut64 baddr = r_bin_get_baddr (r->bin);

	if (IS_MODE_RAD (mode)) {
		r_cons_printf ("fs symbols\n");
	} else if (IS_MODE_JSON (mode)) {
		r_cons_printf ("[");
	} else if (IS_MODE_NORMAL (mode)) {
		if (inifin) {
			r_cons_printf ("[Constructors]\n");
		} else {
			r_cons_printf ("[Entrypoints]\n");
		}
	}

	r_list_foreach (entries, iter, entry) {
		ut64 paddr = entry->paddr;
		ut64 hpaddr = UT64_MAX;
		ut64 hvaddr = UT64_MAX;
		if (mode != R_MODE_SET) {
			if (inifin) {
				if (entry->type == R_BIN_ENTRY_TYPE_PROGRAM) {
					continue;
				}
			} else {
				if (entry->type != R_BIN_ENTRY_TYPE_PROGRAM) {
					continue;
				}
			}
		}
		if (entry->hpaddr) {
			hpaddr = entry->hpaddr;
			if (entry->hvaddr) {
				hvaddr = rva (r->bin, hpaddr, entry->hvaddr, va);
			}
		}
		ut64 at = rva (r->bin, paddr, entry->vaddr, va);
		const char *type = r_bin_entry_type_string (entry->type);
		if (!type) {
			type = "unknown";
		}
		const char *hpaddr_key = (entry->type == R_BIN_ENTRY_TYPE_PROGRAM)
		                ? "haddr" : "hpaddr";
		if (IS_MODE_SET (mode)) {
			r_flag_space_set (r->flags, R_FLAGS_FS_SYMBOLS);
			if (entry->type == R_BIN_ENTRY_TYPE_INIT) {
				snprintf (str, R_FLAG_NAME_SIZE, "entry.init%i", init_i);
			} else if (entry->type == R_BIN_ENTRY_TYPE_FINI) {
				snprintf (str, R_FLAG_NAME_SIZE, "entry.fini%i", fini_i);
			} else if (entry->type == R_BIN_ENTRY_TYPE_PREINIT) {
				snprintf (str, R_FLAG_NAME_SIZE, "entry.preinit%i", preinit_i);
			} else {
				snprintf (str, R_FLAG_NAME_SIZE, "entry%i", i);
			}
			r_flag_set (r->flags, str, at, 1);
			if (is_initfini (entry) && hvaddr != UT64_MAX) {
				r_meta_add (r->anal, R_META_TYPE_DATA, hvaddr,
				            hvaddr + entry->bits / 8, NULL);
			}
		} else if (IS_MODE_SIMPLE (mode)) {
			r_cons_printf ("0x%08"PFMT64x"\n", at);
		} else if (IS_MODE_JSON (mode)) {
			r_cons_printf ("%s{\"vaddr\":%" PFMT64u ","
				"\"paddr\":%" PFMT64u ","
				"\"baddr\":%" PFMT64u ","
				"\"laddr\":%" PFMT64u ",",
				last_processed ? "," : "", at, paddr, baddr, laddr);
			if (hvaddr != UT64_MAX) {
				r_cons_printf ("\"hvaddr\":%" PFMT64u ",", hvaddr);
			}
			r_cons_printf ("\"%s\":%" PFMT64u ","
				"\"type\":\"%s\"}",
				hpaddr_key, hpaddr, type);
		} else if (IS_MODE_RAD (mode)) {
			char *name = NULL;
			if (entry->type == R_BIN_ENTRY_TYPE_INIT) {
				name = r_str_newf ("entry.init%i", init_i);
			} else if (entry->type == R_BIN_ENTRY_TYPE_FINI) {
				name = r_str_newf ("entry.fini%i", fini_i);
			} else if (entry->type == R_BIN_ENTRY_TYPE_PREINIT) {
				name = r_str_newf ("entry.preinit%i", preinit_i);
			} else {
				name = r_str_newf ("entry%i", i);
			}
			char *n = __filterQuotedShell (name);
			r_cons_printf ("\"f %s 1 0x%08"PFMT64x"\"\n", n, at);
			r_cons_printf ("\"f %s_%s 1 0x%08"PFMT64x"\"\n", n, hpaddr_key, hpaddr);
			r_cons_printf ("\"s %s\"\n", n);
			free (n);
			free (name);
		} else {
			r_cons_printf ("vaddr=0x%08"PFMT64x" paddr=0x%08"PFMT64x, at, paddr);
			if (is_initfini (entry) && hvaddr != UT64_MAX) {
				r_cons_printf (" hvaddr=0x%08"PFMT64x, hvaddr);
			}
			r_cons_printf (" %s=", hpaddr_key);
			if (hpaddr == UT64_MAX) {
				r_cons_printf ("%"PFMT64d, hpaddr);
			} else {
				r_cons_printf ("0x%08"PFMT64x, hpaddr);
			}
			if (entry->type == R_BIN_ENTRY_TYPE_PROGRAM && hvaddr != UT64_MAX) {
				r_cons_printf (" hvaddr=0x%08"PFMT64x, hvaddr);
			}
			r_cons_printf (" type=%s\n", type);
		}
		if (entry->type == R_BIN_ENTRY_TYPE_INIT) {
			init_i++;
		} else if (entry->type == R_BIN_ENTRY_TYPE_FINI) {
			fini_i++;
		} else if (entry->type == R_BIN_ENTRY_TYPE_PREINIT) {
			preinit_i++;
		} else {
			i++;
		}
		last_processed = iter;
	}
	if (IS_MODE_SET (mode)) {
		if (entry) {
			ut64 at = rva (r->bin, entry->paddr, entry->vaddr, va);
			r_core_seek (r, at, 0);
		}
	} else if (IS_MODE_JSON (mode)) {
		r_cons_printf ("]");
		r_cons_newline ();
	} else if (IS_MODE_NORMAL (mode)) {
		r_cons_printf ("\n%i entrypoints\n", init_i + fini_i + preinit_i + i);
	}
	return true;
}
