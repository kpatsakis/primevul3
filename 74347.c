static int bin_info(RCore *r, int mode, ut64 laddr) {
	int i, j, v;
	char str[R_FLAG_NAME_SIZE];
	RBinInfo *info = r_bin_get_info (r->bin);
	RBinFile *bf = r_bin_cur (r->bin);
	if (!bf) {
		return false;
	}
	RBinObject *obj = bf->o;
	const char *compiled = NULL;
	bool havecode;

	if (!bf || !info || !obj) {
		if (mode & R_MODE_JSON) {
			r_cons_printf ("{}");
		}
		return false;
	}
	havecode = is_executable (obj) | (obj->entries != NULL);
	compiled = get_compile_time (bf->sdb);

	if (IS_MODE_SET (mode)) {
		r_config_set (r->config, "file.type", info->rclass);
		r_config_set (r->config, "cfg.bigendian",
			      info->big_endian ? "true" : "false");
		if (info->rclass && !strcmp (info->rclass, "fs")) {
		} else {
			if (info->lang) {
				r_config_set (r->config, "bin.lang", info->lang);
			}
			r_config_set (r->config, "asm.os", info->os);
			if (info->rclass && !strcmp (info->rclass, "pe")) {
				r_config_set (r->config, "anal.cpp.abi", "msvc");
			} else {
				r_config_set (r->config, "anal.cpp.abi", "itanium");
			}
			r_config_set (r->config, "asm.arch", info->arch);
			if (info->cpu && *info->cpu) {
				r_config_set (r->config, "asm.cpu", info->cpu);
			}
			r_config_set (r->config, "anal.arch", info->arch);
			snprintf (str, R_FLAG_NAME_SIZE, "%i", info->bits);
			r_config_set (r->config, "asm.bits", str);
			r_config_set (r->config, "asm.dwarf",
				(R_BIN_DBG_STRIPPED & info->dbg_info) ? "false" : "true");
			v = r_anal_archinfo (r->anal, R_ANAL_ARCHINFO_ALIGN);
			if (v != -1) {
				r_config_set_i (r->config, "asm.pcalign", v);
			}
		}
		r_core_anal_type_init (r);
		r_core_anal_cc_init (r);
	} else if (IS_MODE_SIMPLE (mode)) {
		r_cons_printf ("arch %s\n", info->arch);
		if (info->cpu && *info->cpu) {
			r_cons_printf ("cpu %s\n", info->cpu);
		}
		r_cons_printf ("bits %d\n", info->bits);
		r_cons_printf ("os %s\n", info->os);
		r_cons_printf ("endian %s\n", info->big_endian? "big": "little");
		v = r_anal_archinfo (r->anal, R_ANAL_ARCHINFO_MIN_OP_SIZE);
		if (v != -1) {
			r_cons_printf ("minopsz %d\n", v);
		}
		v = r_anal_archinfo (r->anal, R_ANAL_ARCHINFO_MAX_OP_SIZE);
		if (v != -1) {
			r_cons_printf ("maxopsz %d\n", v);
		}
		v = r_anal_archinfo (r->anal, R_ANAL_ARCHINFO_ALIGN);
		if (v != -1) {
			r_cons_printf ("pcalign %d\n", v);
		}
	} else if (IS_MODE_RAD (mode)) {
		if (info->type && !strcmp (info->type, "fs")) {
			r_cons_printf ("e file.type=fs\n");
			r_cons_printf ("m /root %s 0\n", info->arch);
		} else {
			r_cons_printf ("e cfg.bigendian=%s\n"
				"e asm.bits=%i\n"
				"e asm.dwarf=%s\n",
				r_str_bool (info->big_endian),
				info->bits,
				r_str_bool (R_BIN_DBG_STRIPPED &info->dbg_info));
			if (info->lang && *info->lang) {
				r_cons_printf ("e bin.lang=%s\n", info->lang);
			}
			if (info->rclass && *info->rclass) {
				r_cons_printf ("e file.type=%s\n",
					info->rclass);
			}
			if (info->os) {
				r_cons_printf ("e asm.os=%s\n", info->os);
			}
			if (info->arch) {
				r_cons_printf ("e asm.arch=%s\n", info->arch);
			}
			if (info->cpu && *info->cpu) {
				r_cons_printf ("e asm.cpu=%s\n", info->cpu);
			}
			v = r_anal_archinfo (r->anal, R_ANAL_ARCHINFO_ALIGN);
			if (v != -1) {
				r_cons_printf ("e asm.pcalign=%d\n", v);
			}
		}
	} else {
		char *tmp_buf;
		if (IS_MODE_JSON (mode)) {
			r_cons_printf ("{");
		}
		pair_str ("arch", info->arch, mode, false);
		if (info->cpu && *info->cpu) {
			pair_str ("cpu", info->cpu, mode, false);
		}
		pair_ut64x ("baddr", r_bin_get_baddr (r->bin), mode, false);
		pair_ut64 ("binsz", r_bin_get_size (r->bin), mode, false);
		pair_str ("bintype", info->rclass, mode, false);
		pair_int ("bits", info->bits, mode, false);
		pair_bool ("canary", info->has_canary, mode, false);
		if (info->has_retguard != -1) {
			pair_bool ("retguard", info->has_retguard, mode, false);
		}
		pair_str ("class", info->bclass, mode, false);
		if (info->actual_checksum) {
			/* computed checksum */
			pair_str ("cmp.csum", info->actual_checksum, mode, false);
		}
		pair_str ("compiled", compiled, mode, false);
		pair_str ("compiler", info->compiler, mode, false);
		pair_bool ("crypto", info->has_crypto, mode, false);
		pair_str ("dbg_file", info->debug_file_name, mode, false);
		pair_str ("endian", info->big_endian ? "big" : "little", mode, false);
		if (info->rclass && !strcmp (info->rclass, "mdmp")) {
			tmp_buf = sdb_get (bf->sdb, "mdmp.flags", 0);
			if (tmp_buf) {
				pair_str ("flags", tmp_buf, mode, false);
				free (tmp_buf);
			}
		}
		pair_bool ("havecode", havecode, mode, false);
		if (info->claimed_checksum) {
			/* checksum specified in header */
			pair_str ("hdr.csum", info->claimed_checksum, mode, false);
		}
		pair_str ("guid", info->guid, mode, false);
		pair_str ("intrp", info->intrp, mode, false);
		pair_ut64x ("laddr", laddr, mode, false);
		pair_str ("lang", info->lang, mode, false);
		pair_bool ("linenum", R_BIN_DBG_LINENUMS & info->dbg_info, mode, false);
		pair_bool ("lsyms", R_BIN_DBG_SYMS & info->dbg_info, mode, false);
		pair_str ("machine", info->machine, mode, false);
		v = r_anal_archinfo (r->anal, R_ANAL_ARCHINFO_MAX_OP_SIZE);
		if (v != -1) {
			pair_int ("maxopsz", v, mode, false);
		}
		v = r_anal_archinfo (r->anal, R_ANAL_ARCHINFO_MIN_OP_SIZE);
		if (v != -1) {
			pair_int ("minopsz", v, mode, false);
		}
		pair_bool ("nx", info->has_nx, mode, false);
		pair_str ("os", info->os, mode, false);
		if (info->rclass && !strcmp (info->rclass, "pe")) {
			pair_bool ("overlay", info->pe_overlay, mode, false);
		}
		v = r_anal_archinfo (r->anal, R_ANAL_ARCHINFO_ALIGN);
		if (v != -1) {
			pair_int ("pcalign", v, mode, false);
		}
		pair_bool ("pic", info->has_pi, mode, false);
		pair_bool ("relocs", R_BIN_DBG_RELOCS & info->dbg_info, mode, false);
		Sdb *sdb_info = sdb_ns (obj->kv, "info", false);
		tmp_buf = sdb_get (sdb_info, "elf.relro", 0);
		if (tmp_buf) {
			pair_str ("relro", tmp_buf, mode, false);
			free (tmp_buf);
		}
		pair_str ("rpath", info->rpath, mode, false);
		if (info->rclass && !strcmp (info->rclass, "pe")) {
			pair_bool ("signed", info->signature, mode, false);
		}
		pair_bool ("sanitiz", info->has_sanitizers, mode, false);
		pair_bool ("static", r_bin_is_static (r->bin), mode, false);
		if (info->rclass && !strcmp (info->rclass, "mdmp")) {
			v = sdb_num_get (bf->sdb, "mdmp.streams", 0);
			if (v != -1) {
				pair_int ("streams", v, mode, false);
			}
		}
		pair_bool ("stripped", R_BIN_DBG_STRIPPED & info->dbg_info, mode, false);
		pair_str ("subsys", info->subsystem, mode, false);
		pair_bool ("va", info->has_va, mode, true);
		if (IS_MODE_JSON (mode)) {
			r_cons_printf (",\"checksums\":{");
			for (i = 0; info->sum[i].type; i++) {
				RBinHash *h = &info->sum[i];
				ut64 hash = r_hash_name_to_bits (h->type);
				RHash *rh = r_hash_new (true, hash);
				ut8 *tmp = R_NEWS (ut8, h->to);
				if (!tmp) {
					return false;
				}
				r_buf_read_at (bf->buf, h->from, tmp, h->to);
				int len = r_hash_calculate (rh, hash, tmp, h->to);
				free (tmp);
				if (len < 1) {
					eprintf ("Invalid checksum length\n");
				}
				r_hash_free (rh);
				r_cons_printf ("%s\"%s\":{\"hex\":\"", i?",": "", h->type);
				for (j = 0; j < h->len; j++) {
					r_cons_printf ("%02x", h->buf[j]);
				}
				r_cons_printf ("\"}");
			}
			r_cons_printf ("}");
		} else {
			for (i = 0; info->sum[i].type; i++) {
				RBinHash *h = &info->sum[i];
				ut64 hash = r_hash_name_to_bits (h->type);
				RHash *rh = r_hash_new (true, hash);
				ut8 *tmp = R_NEWS (ut8, h->to);
				if (!tmp) {
					return false;
				}
				r_buf_read_at (bf->buf, h->from, tmp, h->to);
				int len = r_hash_calculate (rh, hash, tmp, h->to);
				free (tmp);
				if (len < 1) {
					eprintf ("Invalid wtf\n");
				}
				r_hash_free (rh);
				r_cons_printf ("%s  %d-%dc  ", h->type, h->from, h->to+h->from);
				for (j = 0; j < h->len; j++) {
					r_cons_printf ("%02x", h->buf[j]);
				}
				r_cons_newline ();
			}
		}
		if (IS_MODE_JSON (mode)) {
			r_cons_printf ("}");
		}
	}
	const char *dir_prefix = r_config_get (r->config, "dir.prefix");
	char *spath = sdb_fmt ("%s/"R2_SDB_FCNSIGN"/spec.sdb", dir_prefix);
	if (r_file_exists (spath)) {
		sdb_concat_by_path (r->anal->sdb_fmts, spath);
	}
	return true;
}
