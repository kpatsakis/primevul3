static int bin_classes(RCore *r, int mode) {
	RListIter *iter, *iter2, *iter3;
	RBinSymbol *sym;
	RBinClass *c;
	RBinField *f;
	char *name;
	RList *cs = r_bin_get_classes (r->bin);
	if (!cs) {
		if (IS_MODE_JSON (mode)) {
			r_cons_print ("[]");
		}
		return false;
	}
	if (IS_MODE_JSON (mode)) {
		r_cons_print ("[");
	} else if (IS_MODE_SET (mode)) {
		if (!r_config_get_i (r->config, "bin.classes")) {
			return false;
		}
		r_flag_space_set (r->flags, R_FLAGS_FS_CLASSES);
	} else if (IS_MODE_RAD (mode)) {
		r_cons_println ("fs classes");
	}

	r_list_foreach (cs, iter, c) {
		if (!c || !c->name || !c->name[0]) {
			continue;
		}
		name = strdup (c->name);
		r_name_filter (name, 0);
		ut64 at_min = UT64_MAX;
		ut64 at_max = 0LL;

		r_list_foreach (c->methods, iter2, sym) {
			if (sym->vaddr) {
				if (sym->vaddr < at_min) {
					at_min = sym->vaddr;
				}
				if (sym->vaddr + sym->size > at_max) {
					at_max = sym->vaddr + sym->size;
				}
			}
		}
		if (at_min == UT64_MAX) {
			at_min = c->addr;
			at_max = c->addr; // XXX + size?
		}

		if (IS_MODE_SET (mode)) {
			const char *classname = sdb_fmt ("class.%s", name);
			r_flag_set (r->flags, classname, c->addr, 1);
			r_list_foreach (c->methods, iter2, sym) {
				char *mflags = r_core_bin_method_flags_str (sym->method_flags, mode);
				char *method = sdb_fmt ("method%s.%s.%s",
					mflags, c->name, sym->name);
				R_FREE (mflags);
				r_name_filter (method, -1);
				r_flag_set (r->flags, method, sym->vaddr, 1);
			}
		} else if (IS_MODE_SIMPLEST (mode)) {
			r_cons_printf ("%s\n", c->name);
		} else if (IS_MODE_SIMPLE (mode)) {
			r_cons_printf ("0x%08"PFMT64x" [0x%08"PFMT64x" - 0x%08"PFMT64x"] %s%s%s\n",
				c->addr, at_min, at_max, c->name, c->super ? " " : "",
				c->super ? c->super : "");
		} else if (IS_MODE_RAD (mode)) {
			char *n = __filterShell (name);
			r_cons_printf ("\"f class.%s = 0x%"PFMT64x"\"\n", n, at_min);
			free (n);
			if (c->super) {
				char *cn = c->name; // __filterShell (c->name);
				char *su = c->super; // __filterShell (c->super);
				r_cons_printf ("\"f super.%s.%s = %d\"\n",
						cn, su, c->index);
			}
			r_list_foreach (c->methods, iter2, sym) {
				char *mflags = r_core_bin_method_flags_str (sym->method_flags, mode);
				char *n = c->name; //  __filterShell (c->name);
				char *sn = sym->name; //__filterShell (sym->name);
				char *cmd = r_str_newf ("\"f method%s.%s.%s = 0x%"PFMT64x"\"\n", mflags, n, sn, sym->vaddr);
				if (cmd) {
					r_str_replace_char (cmd, ' ', '_');
					if (strlen (cmd) > 2) {
						cmd[2] = ' ';
					}
					char *eq = (char *)r_str_rchr (cmd, NULL, '=');
					if (eq && eq != cmd) {
						eq[-1] = eq[1] = ' ';
					}
					r_str_replace_char (cmd, '\n', 0);
					r_cons_printf ("%s\n", cmd);
					free (cmd);
				}
				R_FREE (mflags);
			}
		} else if (IS_MODE_JSON (mode)) {
			if (c->super) {
				r_cons_printf ("%s{\"classname\":\"%s\",\"addr\":%"PFMT64d",\"index\":%d,\"visibility\":\"%s\",\"super\":\"%s\",\"methods\":[",
					iter->p ? "," : "", c->name, c->addr,
					c->index, c->visibility_str? c->visibility_str: "", c->super);
			} else {
				r_cons_printf ("%s{\"classname\":\"%s\",\"addr\":%"PFMT64d",\"index\":%d,\"methods\":[",
					iter->p ? "," : "", c->name, c->addr,
					c->index);
			}
			r_list_foreach (c->methods, iter2, sym) {
				if (sym->method_flags) {
					char *mflags = r_core_bin_method_flags_str (sym->method_flags, mode);
					r_cons_printf ("%s{\"name\":\"%s\",\"flags\":%s,\"addr\":%"PFMT64d"}",
						iter2->p? ",": "", sym->name, mflags, sym->vaddr);
					R_FREE (mflags);
				} else {
					r_cons_printf ("%s{\"name\":\"%s\",\"addr\":%"PFMT64d"}",
						iter2->p? ",": "", sym->name, sym->vaddr);
				}
			}
			r_cons_printf ("], \"fields\":[");
			r_list_foreach (c->fields, iter3, f) {
				if (f->flags) {
					char *mflags = r_core_bin_method_flags_str (f->flags, mode);
					r_cons_printf ("%s{\"name\":\"%s\",\"flags\":%s,\"addr\":%"PFMT64d"}",
						iter3->p? ",": "", f->name, mflags, f->vaddr);
					R_FREE (mflags);
				} else {
					r_cons_printf ("%s{\"name\":\"%s\",\"addr\":%"PFMT64d"}",
						iter3->p? ",": "", f->name, f->vaddr);
				}
			}
			r_cons_printf ("]}");
		} else if (IS_MODE_CLASSDUMP (mode)) {
			if (c) {
				RBinFile *bf = r_bin_cur (r->bin);
				if (bf && bf->o) {
					if (bf->o->lang == R_BIN_NM_JAVA || (bf->o->info && bf->o->info->lang && strstr (bf->o->info->lang, "dalvik"))) {
						classdump_java (r, c);
					} else {
						classdump_objc (r, c);
					}
				} else {
					classdump_objc (r, c);
				}
			}
		} else {
			int m = 0;
			r_cons_printf ("0x%08"PFMT64x" [0x%08"PFMT64x" - 0x%08"PFMT64x"] %6"PFMT64d" class %d %s",
				c->addr, at_min, at_max, (at_max - at_min), c->index, c->name);
			if (c->super) {
				r_cons_printf (" :: %s\n", c->super);
			} else {
				r_cons_newline ();
			}
			r_list_foreach (c->methods, iter2, sym) {
				char *mflags = r_core_bin_method_flags_str (sym->method_flags, mode);
				r_cons_printf ("0x%08"PFMT64x" method %d %s %s\n",
					sym->vaddr, m, mflags, sym->dname? sym->dname: sym->name);
				R_FREE (mflags);
				m++;
			}
		}
		free (name);
	}
	if (IS_MODE_JSON (mode)) {
		r_cons_print ("]");
	}

	return true;
}

static int bin_size(RCore *r, int mode) {
	ut64 size = r_bin_get_size (r->bin);
	if (IS_MODE_SIMPLE (mode) || IS_MODE_JSON (mode)) {
		r_cons_printf ("%"PFMT64u"\n", size);
	} else if (IS_MODE_RAD (mode)) {
		r_cons_printf ("f bin_size @ %"PFMT64u"\n", size);
	} else if (IS_MODE_SET (mode)) {
		r_core_cmdf (r, "f bin_size @ %"PFMT64u"\n", size);
	} else {
		r_cons_printf ("%"PFMT64u"\n", size);
	}
	return true;
}

static int bin_libs(RCore *r, int mode) {
	RList *libs;
	RListIter *iter;
	char* lib;
	int i = 0;

	if (!(libs = r_bin_get_libs (r->bin))) {
		return false;
	}
	if (IS_MODE_JSON (mode)) {
		r_cons_print ("[");
	} else if (IS_MODE_NORMAL (mode)) {
		r_cons_println ("[Linked libraries]");
	}
	r_list_foreach (libs, iter, lib) {
		if (IS_MODE_SET (mode)) {
		} else if (IS_MODE_RAD (mode)) {
			r_cons_printf ("\"CCa entry0 %s\"\n", lib);
		} else if (IS_MODE_JSON (mode)) {
			r_cons_printf ("%s\"%s\"", iter->p ? "," : "", lib);
		} else {
			r_cons_println (lib);
		}
		i++;
	}
	if (IS_MODE_JSON (mode)) {
		r_cons_print ("]");
	} else if (IS_MODE_NORMAL (mode)) {
		if (i == 1) {
			r_cons_printf ("\n%i library\n", i);
		} else {
			r_cons_printf ("\n%i libraries\n", i);
		}
	}
	return true;
}

static void bin_mem_print(RList *mems, int perms, int depth, int mode) {
	RBinMem *mem;
	RListIter *iter;
	if (!mems) {
		return;
	}
	r_list_foreach (mems, iter, mem) {
		if (IS_MODE_JSON (mode)) {
			r_cons_printf ("{\"name\":\"%s\",\"size\":%d,\"address\":%d,"
					"\"flags\":\"%s\"}", mem->name, mem->size,
					mem->addr, r_str_rwx_i (mem->perms & perms));
		} else if (IS_MODE_SIMPLE (mode)) {
			r_cons_printf ("0x%08"PFMT64x"\n", mem->addr);
		} else {
			r_cons_printf ("0x%08"PFMT64x" +0x%04x %s %*s%-*s\n",
					mem->addr, mem->size, r_str_rwx_i (mem->perms & perms),
					depth, "", 20-depth, mem->name);
		}
		if (mem->mirrors) {
			if (IS_MODE_JSON (mode)) {
				r_cons_printf (",");
			}
			bin_mem_print (mem->mirrors, mem->perms & perms, depth + 1, mode);
		}
		if (IS_MODE_JSON (mode)) {
			if (iter->n) {
				r_cons_printf (",");
			}
		}
	}
}

static int bin_mem(RCore *r, int mode) {
	RList *mem = NULL;
	if (!r) {
		return false;
	}
	if (!IS_MODE_JSON (mode)) {
		if (!(IS_MODE_RAD (mode) || IS_MODE_SET (mode))) {
			r_cons_println ("[Memory]\n");
		}
	}
	if (!(mem = r_bin_get_mem (r->bin))) {
		if (IS_MODE_JSON (mode)) {
			r_cons_print("[]");
		}
		return false;
	}
	if (IS_MODE_JSON (mode)) {
		r_cons_print ("[");
		bin_mem_print (mem, 7, 0, R_MODE_JSON);
		r_cons_println ("]");
		return true;
	} else if (!(IS_MODE_RAD (mode) || IS_MODE_SET (mode))) {
		bin_mem_print (mem, 7, 0, mode);
	}
	return true;
}

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
}
