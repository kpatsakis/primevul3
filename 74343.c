static int bin_dwarf(RCore *core, int mode) {
	RBinDwarfRow *row;
	RListIter *iter;
	RList *list = NULL;
	if (!r_config_get_i (core->config, "bin.dbginfo")) {
		return false;
	}
	RBinFile *binfile = r_bin_cur (core->bin);
	RBinPlugin * plugin = r_bin_file_cur_plugin (binfile);
	if (!binfile) {
		return false;
	}
	if (plugin && plugin->lines) {
		list = plugin->lines (binfile);
	} else if (core->bin) {
		RBinDwarfDebugAbbrev *da = NULL;
		da = r_bin_dwarf_parse_abbrev (core->bin, mode);
		r_bin_dwarf_parse_info (da, core->bin, mode);
		r_bin_dwarf_parse_aranges (core->bin, mode);
		list = r_bin_dwarf_parse_line (core->bin, mode);
		r_bin_dwarf_free_debug_abbrev (da);
		free (da);
	}
	if (!list) {
		return false;
	}

	r_cons_break_push (NULL, NULL);
	/* cache file:line contents */
	const char *lastFile = NULL;
	int *lastFileLines = NULL;
	char *lastFileContents = NULL;
	int lastFileLinesCount = 0;

	/* ugly dupe for speedup */
	const char *lastFile2 = NULL;
	int *lastFileLines2 = NULL;
	char *lastFileContents2 = NULL;
	int lastFileLinesCount2 = 0;

	const char *lf = NULL;
	int *lfl = NULL;
	char *lfc = NULL;
	int lflc = 0;

	r_list_foreach (list, iter, row) {
		if (r_cons_is_breaked ()) {
			break;
		}
		if (mode) {
			const char *path = row->file;
			if (!lastFile || strcmp (path, lastFile)) {
				if (lastFile && lastFile2 && !strcmp (path, lastFile2)) {
					lf = lastFile;
					lfl = lastFileLines;
					lfc = lastFileContents;
					lflc = lastFileLinesCount;
					lastFile = lastFile2;
					lastFileLines = lastFileLines2;
					lastFileContents = lastFileContents2;
					lastFileLinesCount = lastFileLinesCount2;
					lastFile2 = lf;
					lastFileLines2 = lfl;
					lastFileContents2 = lfc;
					lastFileLinesCount2 = lflc;
				} else {
					lastFile2 = lastFile;
					lastFileLines2 = lastFileLines;
					lastFileContents2 = lastFileContents;
					lastFileLinesCount2 = lastFileLinesCount;
					lastFile = path;
					lastFileContents = r_file_slurp (path, NULL);
					if (lastFileContents) {
						lastFileLines = r_str_split_lines (lastFileContents, &lastFileLinesCount);
					}
				}
			}
			char *line = NULL;
			if (lastFileLines && lastFileContents) {
				int nl = row->line - 1;
				if (nl >= 0 && nl < lastFileLinesCount) {
					line = strdup (lastFileContents + lastFileLines[nl]);
				}
			} else {
				line = NULL;
			}
			if (line) {
				r_str_filter (line, strlen (line));
				line = r_str_replace (line, "\"", "\\\"", 1);
				line = r_str_replace (line, "\\\\", "\\", 1);
			}
			bool chopPath = !r_config_get_i (core->config, "dir.dwarf.abspath");
			char *file = strdup (row->file);
			if (chopPath) {
				const char *slash = r_str_lchr (file, '/');
				if (slash) {
					memmove (file, slash + 1, strlen (slash));
				}
			}
			if ((mode & R_MODE_SET)) {
#if 0
				char *cmt = r_str_newf ("%s:%d %s", file, (int)row->line, line? line: "");
				r_meta_set_string (core->anal, R_META_TYPE_COMMENT, row->address, cmt);
				free (cmt);
#endif
			} else {
				r_cons_printf ("CL %s:%d 0x%08" PFMT64x "\n",
					       file, (int)row->line,
					       row->address);
				r_cons_printf ("\"CC %s:%d %s\"@0x%" PFMT64x
					       "\n",
					       file, row->line,
					       line ? line : "", row->address);
			}
			free (file);
			free (line);
		} else {
			r_cons_printf ("0x%08" PFMT64x "\t%s\t%d\n",
				       row->address, row->file, row->line);
		}
	}
	r_cons_break_pop ();
	R_FREE (lastFileContents);
	R_FREE (lastFileContents2);
	free (lastFileLines);
	return true;
}
