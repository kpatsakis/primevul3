static int bin_relocs(RCore *r, int mode, int va) {
	bool bin_demangle = r_config_get_i (r->config, "bin.demangle");
	bool keep_lib = r_config_get_i (r->config, "bin.demangle.libs");
	const char *lang = r_config_get (r->config, "bin.lang");
	RBIter iter;
	RBinReloc *reloc = NULL;
	Sdb *db = NULL;
	PJ *pj = NULL;
	char *sdb_module = NULL;
	int i = 0;

	R_TIME_BEGIN;

	va = VA_TRUE; // XXX relocs always vaddr?
	RBNode *relocs = r_bin_patch_relocs (r->bin);
	if (!relocs) {
		relocs = r_bin_get_relocs (r->bin);
	}

	if (IS_MODE_RAD (mode)) {
		r_cons_println ("fs relocs");
	} else if (IS_MODE_NORMAL (mode)) {
		r_cons_println ("[Relocations]");
	} else if (IS_MODE_JSON (mode)) {
		pj = pj_new ();
		if (pj) {
			pj_a (pj);
		}
	} else if (IS_MODE_SET (mode)) {
		r_flag_space_set (r->flags, R_FLAGS_FS_RELOCS);
	}

	r_rbtree_foreach (relocs, iter, reloc, RBinReloc, vrb) {
		ut64 addr = rva (r->bin, reloc->paddr, reloc->vaddr, va);
		if (IS_MODE_SET (mode) && (is_section_reloc (reloc) || is_file_reloc (reloc))) {
			/*
			 * Skip section reloc because they will have their own flag.
			 * Skip also file reloc because not useful for now.
			 */
		} else if (IS_MODE_SET (mode)) {
			set_bin_relocs (r, reloc, addr, &db, &sdb_module);
			add_metadata (r, reloc, addr, mode);
		} else if (IS_MODE_SIMPLE (mode)) {
			r_cons_printf ("0x%08"PFMT64x"  %s\n", addr, reloc->import ? reloc->import->name : "");
		} else if (IS_MODE_RAD (mode)) {
			char *name = reloc->import
				? strdup (reloc->import->name)
				: (reloc->symbol ? strdup (reloc->symbol->name) : NULL);
			if (name && bin_demangle) {
				char *mn = r_bin_demangle (r->bin->cur, NULL, name, addr, keep_lib);
				if (mn) {
					free (name);
					name = mn;
				}
			}
			if (name) {
				int reloc_size = 4;
				char *n = __filterQuotedShell (name);
				r_cons_printf ("\"f %s%s%s %d 0x%08"PFMT64x"\"\n",
					r->bin->prefix ? r->bin->prefix : "reloc.",
					r->bin->prefix ? "." : "", n, reloc_size, addr);
				add_metadata (r, reloc, addr, mode);
				free (n);
				free (name);
			}
		} else if (IS_MODE_JSON (mode)) {
			if (pj) {
				pj_o (pj);
				char *mn = NULL;
				char *relname = NULL;

				if (reloc->import) {
					mn = r_bin_demangle (r->bin->cur, lang, reloc->import->name, addr, keep_lib);
					relname = strdup (reloc->import->name);
				} else if (reloc->symbol) {
					mn = r_bin_demangle (r->bin->cur, lang, reloc->symbol->name, addr, keep_lib);
					relname = strdup (reloc->symbol->name);
				}

				pj_ks (pj, "name", (relname && strcmp (relname, "")) ? relname : "N/A");
				pj_ks (pj, "demname", mn ? mn : "");
				pj_ks (pj, "type", bin_reloc_type_name (reloc));
				pj_kn (pj, "vaddr", reloc->vaddr);
				pj_kn (pj, "paddr", reloc->paddr);
				if (reloc->symbol) {
					pj_kn (pj, "sym_va", reloc->symbol->vaddr);
				}
				pj_kb (pj, "is_ifunc", reloc->is_ifunc);
				pj_end (pj);

				free (mn);
				if (relname) {
					free (relname);
				}
			}
		} else if (IS_MODE_NORMAL (mode)) {
			char *name = reloc->import
				? strdup (reloc->import->name)
				: reloc->symbol
				? strdup (reloc->symbol->name)
				: strdup ("null");
			if (bin_demangle) {
				char *mn = r_bin_demangle (r->bin->cur, NULL, name, addr, keep_lib);
				if (mn && *mn) {
					free (name);
					name = mn;
				}
			}
			r_cons_printf ("vaddr=0x%08"PFMT64x" paddr=0x%08"PFMT64x" type=%s",
				addr, reloc->paddr, bin_reloc_type_name (reloc));
			if (reloc->import && reloc->import->name[0]) {
				r_cons_printf (" %s", name);
			} else if (reloc->symbol && name && name[0]) {
				r_cons_printf (" %s", name);
			}
			R_FREE (name);
			if (reloc->addend) {
				if ((reloc->import || (reloc->symbol && !R_STR_ISEMPTY (name))) && reloc->addend > 0) {
					r_cons_printf (" +");
				}
				if (reloc->addend < 0) {
					r_cons_printf (" - 0x%08"PFMT64x, -reloc->addend);
				} else {
					r_cons_printf (" 0x%08"PFMT64x, reloc->addend);
				}
			}
			if (reloc->is_ifunc) {
				r_cons_print (" (ifunc)");
			}
			r_cons_newline ();
		}
		i++;
	}
	if (IS_MODE_JSON (mode)) {
		pj_end (pj);
		r_cons_println (pj_string (pj));
	}
	if (IS_MODE_NORMAL (mode)) {
		r_cons_printf ("\n%i relocations\n", i);
	}

	if (pj) {
		pj_free (pj);
	}
	R_FREE (sdb_module);
	sdb_free (db);
	db = NULL;

	R_TIME_END;
	return relocs != NULL;
}
