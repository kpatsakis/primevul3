static int parse_import_stub(struct MACH0_(obj_t)* bin, struct symbol_t *symbol, int idx) {
	int i, j, nsyms, stridx;
	const char *symstr;
	if (idx < 0) {
		return 0;
	}
	symbol->offset = 0LL;
	symbol->addr = 0LL;
	symbol->name[0] = '\0';

	if (!bin || !bin->sects) {
		return false;
	}
	for (i = 0; i < bin->nsects; i++) {
		if ((bin->sects[i].flags & SECTION_TYPE) == S_SYMBOL_STUBS && bin->sects[i].reserved2 > 0) {
			nsyms = (int)(bin->sects[i].size / bin->sects[i].reserved2);
			if (nsyms > bin->size) {
				bprintf ("mach0: Invalid symbol table size\n");
			}
			for (j = 0; j < nsyms; j++) {
				if (bin->sects) {
					if (bin->sects[i].reserved1 + j >= bin->nindirectsyms) {
						continue;
					}
				}
				if (bin->indirectsyms) {
					if (idx != bin->indirectsyms[bin->sects[i].reserved1 + j]) {
						continue;
					}
				}
				if (idx > bin->nsymtab) {
					continue;
				}
				symbol->type = R_BIN_MACH0_SYMBOL_TYPE_LOCAL;
				symbol->offset = bin->sects[i].offset + j * bin->sects[i].reserved2;
				symbol->addr = bin->sects[i].addr + j * bin->sects[i].reserved2;
				symbol->size = 0;
				stridx = bin->symtab[idx].n_strx;
				if (stridx >= 0 && stridx < bin->symstrlen) {
					symstr = (char *)bin->symstr + stridx;
				} else {
					symstr = "???";
				}
				if (*symstr == '_') {
					symstr++;
				}
				snprintf (symbol->name, R_BIN_MACH0_STRING_LENGTH, "imp.%s", symstr);
				return true;
			}
		}
	}
	return false;
}
