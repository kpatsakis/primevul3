static void _print_strings(RCore *r, RList *list, int mode, int va) {
	bool b64str = r_config_get_i (r->config, "bin.b64str");
	int minstr = r_config_get_i (r->config, "bin.minstr");
	int maxstr = r_config_get_i (r->config, "bin.maxstr");
	RBin *bin = r->bin;
	RBinObject *obj = r_bin_cur_object (bin);
	RListIter *iter;
	RListIter *last_processed = NULL;
	RBinString *string;
	RBinSection *section;
	char *q;

	bin->minstrlen = minstr;
	bin->maxstrlen = maxstr;
	if (IS_MODE_JSON (mode)) {
		r_cons_printf ("[");
	}
	if (IS_MODE_RAD (mode)) {
		r_cons_println ("fs strings");
	}
	if (IS_MODE_SET (mode) && r_config_get_i (r->config, "bin.strings")) {
		r_flag_space_set (r->flags, R_FLAGS_FS_STRINGS);
		r_cons_break_push (NULL, NULL);
	}
	if (IS_MODE_NORMAL (mode)) {
		r_cons_printf ("[Strings]\n");
		r_cons_printf ("Num Paddr      Vaddr      Len Size Section  Type  String\n");
	}
	RBinString b64 = { 0 };
	r_list_foreach (list, iter, string) {
		const char *section_name, *type_string;
		ut64 paddr, vaddr;
		paddr = string->paddr;
		vaddr = rva (r->bin, paddr, string->vaddr, va);
		if (!r_bin_string_filter (bin, string->string, vaddr)) {
			continue;
		}
		if (string->length < minstr) {
			continue;
		}
		if (maxstr && string->length > maxstr) {
			continue;
		}

		section = obj? r_bin_get_section_at (obj, paddr, 0): NULL;
		section_name = section ? section->name : "";
		type_string = r_bin_string_type (string->type);
		if (b64str) {
			ut8 *s = r_base64_decode_dyn (string->string, -1);
			if (s && *s && IS_PRINTABLE (*s)) {
				free (b64.string);
				memcpy (&b64, string, sizeof (b64));
				b64.string = (char *)s;
				b64.size = strlen (b64.string);
				string = &b64;
			}
		}
		if (IS_MODE_SET (mode)) {
			char *f_name, *f_realname, *str;
			if (r_cons_is_breaked ()) {
				break;
			}
			r_meta_add (r->anal, R_META_TYPE_STRING, vaddr, vaddr + string->size, string->string);
			f_name = strdup (string->string);
			r_name_filter (f_name, -1);
			if (r->bin->prefix) {
				str = r_str_newf ("%s.str.%s", r->bin->prefix, f_name);
				f_realname = r_str_newf ("%s.\"%s\"", r->bin->prefix, string->string);
			} else {
				str = r_str_newf ("str.%s", f_name);
				f_realname = r_str_newf ("\"%s\"", string->string);
			}
			RFlagItem *flag = r_flag_set (r->flags, str, vaddr, string->size);
			r_flag_item_set_realname (flag, f_realname);
			free (str);
			free (f_name);
			free (f_realname);
		} else if (IS_MODE_SIMPLE (mode)) {
			r_cons_printf ("0x%"PFMT64x" %d %d %s\n", vaddr,
				string->size, string->length, string->string);
		} else if (IS_MODE_SIMPLEST (mode)) {
			r_cons_println (string->string);
		} else if (IS_MODE_JSON (mode)) {
			int *block_list;
			q = r_base64_encode_dyn (string->string, -1);
			r_cons_printf ("%s{\"vaddr\":%"PFMT64u
				",\"paddr\":%"PFMT64u",\"ordinal\":%d"
				",\"size\":%d,\"length\":%d,\"section\":\"%s\","
				"\"type\":\"%s\",\"string\":\"%s\"",
				last_processed ? ",": "",
				vaddr, paddr, string->ordinal, string->size,
				string->length, section_name, type_string, q);
			switch (string->type) {
			case R_STRING_TYPE_UTF8:
			case R_STRING_TYPE_WIDE:
			case R_STRING_TYPE_WIDE32:
				block_list = r_utf_block_list ((const ut8*)string->string, -1, NULL);
				if (block_list) {
					if (block_list[0] == 0 && block_list[1] == -1) {
						/* Don't include block list if
						   just Basic Latin (0x00 - 0x7F) */
						R_FREE (block_list);
						break;
					}
					int *block_ptr = block_list;
					r_cons_printf (",\"blocks\":[");
					for (; *block_ptr != -1; block_ptr++) {
						if (block_ptr != block_list) {
							r_cons_printf (",");
						}
						const char *utfName = r_utf_block_name (*block_ptr);
						r_cons_printf ("\"%s\"", utfName? utfName: "");
					}
					r_cons_printf ("]");
					R_FREE (block_list);
				}
			}
			r_cons_printf ("}");
			free (q);
		} else if (IS_MODE_RAD (mode)) {
			char *f_name = strdup (string->string);
			r_name_filter (f_name, R_FLAG_NAME_SIZE);
			char *str = (r->bin->prefix)
				? r_str_newf ("%s.str.%s", r->bin->prefix, f_name)
				: r_str_newf ("str.%s", f_name);
			r_cons_printf ("f %s %"PFMT64d" 0x%08"PFMT64x"\n"
				"Cs %"PFMT64d" @ 0x%08"PFMT64x"\n",
				str, string->size, vaddr,
				string->size, vaddr);
			free (str);
			free (f_name);
		} else {
			int *block_list;
			char *str = string->string;
			char *no_dbl_bslash_str = NULL;
			if (!r->print->esc_bslash) {
				char *ptr;
				for (ptr = str; *ptr; ptr++) {
					if (*ptr != '\\') {
						continue;
					}
					if (*(ptr + 1) == '\\') {
						if (!no_dbl_bslash_str) {
							no_dbl_bslash_str = strdup (str);
							if (!no_dbl_bslash_str) {
								break;
							}
							ptr = no_dbl_bslash_str + (ptr - str);
						}
						memmove (ptr + 1, ptr + 2, strlen (ptr + 2) + 1);
					}
				}
				if (no_dbl_bslash_str) {
					str = no_dbl_bslash_str;
				}
			}
			r_cons_printf ("%03u 0x%08" PFMT64x " 0x%08" PFMT64x " %3u %3u (%s) %5s %s",
				string->ordinal, paddr, vaddr,
				string->length, string->size,
				section_name, type_string, str);
			if (str == no_dbl_bslash_str) {
				R_FREE (str);
			}
			switch (string->type) {
			case R_STRING_TYPE_UTF8:
			case R_STRING_TYPE_WIDE:
			case R_STRING_TYPE_WIDE32:
				block_list = r_utf_block_list ((const ut8*)string->string, -1, NULL);
				if (block_list) {
					if (block_list[0] == 0 && block_list[1] == -1) {
						/* Don't show block list if
						   just Basic Latin (0x00 - 0x7F) */
						break;
					}
					int *block_ptr = block_list;
					r_cons_printf (" blocks=");
					for (; *block_ptr != -1; block_ptr++) {
						if (block_ptr != block_list) {
							r_cons_printf (",");
						}
						const char *name = r_utf_block_name (*block_ptr);
						r_cons_printf ("%s", name? name: "");
					}
					free (block_list);
				}
				break;
			}
			r_cons_printf ("\n");
		}
		last_processed = iter;
	}
	R_FREE (b64.string);
	if (IS_MODE_JSON (mode)) {
		r_cons_printf ("]");
	}
	if (IS_MODE_SET (mode)) {
		r_cons_break_pop ();
	}
}
