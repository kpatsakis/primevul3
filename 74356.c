static int bin_sections(RCore *r, int mode, ut64 laddr, int va, ut64 at, const char *name, const char *chksum, bool print_segments) {
	char *str = NULL;
	RBinSection *section;
	RBinInfo *info = NULL;
	RList *sections;
	RListIter *iter;
	RListIter *last_processed = NULL;
	int i = 0;
	int fd = -1;
	bool printHere = false;
	sections = r_bin_get_sections (r->bin);
#if LOAD_BSS_MALLOC
	bool inDebugger = r_config_get_i (r->config, "cfg.debug");
#endif
	HtPP *dup_chk_ht = ht_pp_new0 ();
	bool ret = false;
	const char *type = print_segments ? "segment" : "section";
	bool segments_only = true;
	RList *io_section_info = NULL;

	if (!dup_chk_ht) {
		return false;
	}

	if (chksum && *chksum == '.') {
		printHere = true;
	}
	if (IS_MODE_EQUAL (mode)) {
		int cols = r_cons_get_size (NULL);
		RList *list = r_list_newf ((RListFree) r_listinfo_free);
		if (!list) {
			return false;
		}
		RBinSection *s;
		r_list_foreach (sections, iter, s) {
			char humansz[8];
			if (print_segments != s->is_segment) {
				continue;
			}
			RInterval pitv = (RInterval){s->paddr, s->size};
			RInterval vitv = (RInterval){s->vaddr, s->vsize};
			r_num_units (humansz, sizeof (humansz), s->size);
			RListInfo *info = r_listinfo_new (s->name, pitv, vitv, s->perm, strdup (humansz));
			r_list_append (list, info);
		}
		r_core_visual_list (r, list, r->offset, -1, cols, r->print->flags & R_PRINT_FLAGS_COLOR);
		r_list_free (list);
		goto out;
	}
	if (IS_MODE_JSON (mode) && !printHere) {
		r_cons_printf ("[");
	} else if (IS_MODE_RAD (mode) && !at) {
		r_cons_printf ("fs %ss\n", type);
	} else if (IS_MODE_NORMAL (mode) && !at && !printHere) {
		r_cons_printf ("[%s]\n", print_segments ? "Segments" : "Sections");
	} else if (IS_MODE_NORMAL (mode) && printHere) {
		r_cons_printf ("Current section\n");
	} else if (IS_MODE_SET (mode)) {
		fd = r_core_file_cur_fd (r);
		r_flag_space_set (r->flags, print_segments? R_FLAGS_FS_SEGMENTS: R_FLAGS_FS_SECTIONS);
	}
	if (IS_MODE_NORMAL (mode)) {
		r_cons_printf ("Nm Paddr       Size Vaddr      Memsz Perms %sName\n",
                   chksum ? "Checksum          " : "");
	}
	if (IS_MODE_SET (mode)) {
		r_list_foreach (sections, iter, section) {
			if (!section->is_segment) {
				segments_only = false;
				break;
			}
		}

		io_section_info = r_list_newf ((RListFree)free);
	}
	r_list_foreach (sections, iter, section) {
		char perms[] = "----";
		int va_sect = va;
		ut64 addr;

		if (va && !(section->perm & R_PERM_R)) {
			va_sect = VA_NOREBASE;
		}
		addr = rva (r->bin, section->paddr, section->vaddr, va_sect);

		if (name && strcmp (section->name, name)) {
			continue;
		}

		if ((printHere && !(section->paddr <= r->offset && r->offset < (section->paddr + section->size)))
				&& (printHere && !(addr <= r->offset && r->offset < (addr + section->size)))) {
			continue;
		}

		r_name_filter (section->name, strlen (section->name) + 1);
		if (at && (!section->size || !is_in_range (at, addr, section->size))) {
			continue;
		}

		if (section->is_segment != print_segments) {
			continue;
		}

		if (section->perm & R_PERM_SHAR) {
			perms[0] = 's';
		}
		if (section->perm & R_PERM_R) {
			perms[1] = 'r';
		}
		if (section->perm & R_PERM_W) {
			perms[2] = 'w';
		}
		if (section->perm & R_PERM_X) {
			perms[3] = 'x';
		}
		const char *arch = NULL;
		int bits = 0;
		if (section->arch || section->bits) {
			arch = section->arch;
			bits = section->bits;
		}
		if (info) {
			if (!arch) {
				arch = info->arch;
			}
			if (!bits) {
				bits = info->bits;
			}
		}
		if (!arch) {
			arch = r_config_get (r->config, "asm.arch");
		}
		if (!bits) {
			bits = R_SYS_BITS;
		}
		if (IS_MODE_RAD (mode)) {
			char *n = __filterQuotedShell (section->name);
			r_cons_printf ("\"f %s.%s 1 0x%08"PFMT64x"\"\n", type, n, section->vaddr);
			free (n);
		} else if (IS_MODE_SET (mode)) {
#if LOAD_BSS_MALLOC
			if (!strcmp (section->name, ".bss")) {
				int loaded = 0;
				RListIter *iter;
				RIOMap *m;
				r_list_foreach (r->io->maps, iter, m) {
					if (m->from == addr) {
						loaded = 1;
					}
				}
				if (!loaded && !inDebugger) {
					r_core_cmdf (r, "on malloc://%d 0x%"PFMT64x" # bss\n",
						section->vsize, addr);
				}
			}
#endif
			if (section->format) {
				if (section->vsize < 1024 * 1024 * 2) {
					r_core_cmdf (r, "%s @ 0x%"PFMT64x, section->format, section->vaddr);
				}
			}
			if (r->bin->prefix) {
				str = r_str_newf ("%s.%s.%s", r->bin->prefix, type, section->name);
			} else {
				str = r_str_newf ("%s.%s", type, section->name);

			}
			ut64 size = r->io->va? section->vsize: section->size;
			r_flag_set (r->flags, str, addr, size);
			R_FREE (str);

			if (!section->is_segment || segments_only) {
				char *pfx = r->bin->prefix;
				str = r_str_newf ("[%02d] %s %s size %" PFMT64d" named %s%s%s",
				                  i, perms, type, size,
				                  pfx? pfx: "", pfx? ".": "", section->name);
				r_meta_add (r->anal, R_META_TYPE_COMMENT, addr, addr, str);
				R_FREE (str);
			}
			if (section->add) {
				bool found;
				str = r_str_newf ("%"PFMT64x".%"PFMT64x".%"PFMT64x".%"PFMT64x".%"PFMT32u".%s.%"PFMT32u".%d",
					section->paddr, addr, section->size, section->vsize, section->perm, section->name, r->bin->cur->id, fd);
				ht_pp_find (dup_chk_ht, str, &found);
				if (!found) {
					struct io_bin_section_info_t *ibs = R_NEW (struct io_bin_section_info_t);
					if (!ibs) {
						eprintf ("Could not allocate memory\n");
						goto out;
					}

					ibs->sec = section;
					ibs->addr = addr;
					ibs->fd = fd;
					r_list_append (io_section_info, ibs);
					ht_pp_insert (dup_chk_ht, str, NULL);
				}
				R_FREE (str);
			}
		} else if (IS_MODE_SIMPLE (mode)) {
			char *hashstr = NULL;
			if (chksum) {
				ut8 *data = malloc (section->size);
				if (!data) {
					goto out;
				}
				ut32 datalen = section->size;
				r_io_pread_at (r->io, section->paddr, data, datalen);
				hashstr = build_hash_string (mode, chksum,
							data, datalen);
				free (data);
			}
			r_cons_printf ("0x%"PFMT64x" 0x%"PFMT64x" %s %s%s%s\n",
				addr, addr + section->size,
				perms,
				hashstr ? hashstr : "", hashstr ? " " : "",
				section->name
			);
			free (hashstr);
		} else if (IS_MODE_JSON (mode)) {
			char *hashstr = NULL;
			if (chksum && section->size > 0) {
				ut8 *data = malloc (section->size);
				if (!data) {
					goto out;
				}
				ut32 datalen = section->size;
				r_io_pread_at (r->io, section->paddr, data, datalen);
				hashstr = build_hash_string (mode, chksum,
							data, datalen);
				free (data);
			}
			r_cons_printf ("%s{\"name\":\"%s\","
				"\"size\":%"PFMT64d","
				"\"vsize\":%"PFMT64d","
				"\"perm\":\"%s\","
				"%s"
				"\"paddr\":%"PFMT64d","
				"\"vaddr\":%"PFMT64d"}",
				(last_processed && !printHere) ? "," : "",
				section->name,
				section->size,
				section->vsize,
				perms,
				hashstr ? hashstr : "",
				section->paddr,
				addr);
			free (hashstr);
		} else {
			char *hashstr = NULL, str[128];
			if (chksum) {
				ut8 *data = malloc (section->size);
				if (!data) {
					goto out;
				}
				ut32 datalen = section->size;
				if (datalen > 0) {
					r_io_pread_at (r->io, section->paddr, data, datalen);
				}
				hashstr = build_hash_string (mode, chksum, data, datalen);
				free (data);
			}
			if (section->arch || section->bits) {
				snprintf (str, sizeof (str), "arch=%s bits=%d ",
					r_str_get2 (arch), bits);
			} else {
				str[0] = 0;
			}
			if (r->bin->prefix) {
				r_cons_printf ("%02i 0x%08"PFMT64x" %5"PFMT64d" 0x%08"PFMT64x" %5"PFMT64d" "
					"%s %s%s%s.%s\n",
					i, section->paddr, section->size, addr, section->vsize,
					perms, str, hashstr ?hashstr : "", r->bin->prefix, section->name);
			} else {
				r_cons_printf ("%02i 0x%08"PFMT64x" %5"PFMT64d" 0x%08"PFMT64x" %5"PFMT64d" "
					"%s %s%s%s\n",
					i, section->paddr, (ut64)section->size, addr, (ut64)section->vsize,
					perms, str, hashstr ?hashstr : "", section->name);
			}
			free (hashstr);
		}
		i++;
		last_processed = iter;
		if (printHere) {
			break;
		}
	}
	if (IS_MODE_SET (mode) && !r_io_desc_is_dbg (r->io->desc)) {
		RListIter *it;
		struct io_bin_section_info_t *ibs;
		r_list_foreach_prev (io_section_info, it, ibs) {
			add_section (r, ibs->sec, ibs->addr, ibs->fd);
		}
		r_io_update (r->io);
		r_list_free (io_section_info);
		io_section_info = NULL;
	}
	if (IS_MODE_JSON (mode) && !printHere) {
		r_cons_println ("]");
	} else if (IS_MODE_NORMAL (mode) && !at && !printHere) {
	}

	ret = true;
out:
	ht_pp_free (dup_chk_ht);
	return ret;
}
