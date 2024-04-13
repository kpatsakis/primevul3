static VarFileInfo* Pe_r_bin_pe_parse_var_file_info(struct PE_(r_bin_pe_obj_t)* bin, PE_DWord* curAddr) {
	VarFileInfo* varFileInfo = calloc (1, sizeof(*varFileInfo));
	if (!varFileInfo) {
		bprintf ("Warning: calloc (VarFileInfo)\n");
		return NULL;
	}
	PE_DWord startAddr = *curAddr;
	if (r_buf_read_at (bin->b, *curAddr, (ut8*) &varFileInfo->wLength, sizeof(varFileInfo->wLength)) != sizeof(varFileInfo->wLength)) {
		bprintf ("Warning: read (VarFileInfo wLength)\n");
		free_VarFileInfo (varFileInfo);
		return NULL;
	}
	*curAddr += sizeof(varFileInfo->wLength);

	if (r_buf_read_at (bin->b, *curAddr, (ut8*) &varFileInfo->wValueLength, sizeof(varFileInfo->wValueLength)) != sizeof(varFileInfo->wValueLength)) {
		bprintf ("Warning: read (VarFileInfo wValueLength)\n");
		free_VarFileInfo (varFileInfo);
		return NULL;
	}
	*curAddr += sizeof(varFileInfo->wValueLength);

	if (varFileInfo->wValueLength != 0) {
		bprintf ("Warning: check (VarFileInfo wValueLength)\n");
		free_VarFileInfo (varFileInfo);
		return NULL;
	}

	if (r_buf_read_at (bin->b, *curAddr, (ut8*) &varFileInfo->wType, sizeof(varFileInfo->wType)) != sizeof(varFileInfo->wType)) {
		bprintf ("Warning: read (VarFileInfo wType)\n");
		free_VarFileInfo (varFileInfo);
		return NULL;
	}
	*curAddr += sizeof(varFileInfo->wType);
	if (varFileInfo->wType && varFileInfo->wType != 1) {
		bprintf ("Warning: check (VarFileInfo wType)\n");
		free_VarFileInfo (varFileInfo);
		return NULL;
	}

	varFileInfo->szKey = (ut16*) malloc (UT16_ALIGN (VARFILEINFO_UTF_16_LEN ));  //L"VarFileInfo"
	if (!varFileInfo->szKey) {
		bprintf ("Warning: malloc (VarFileInfo szKey)\n");
		free_VarFileInfo (varFileInfo);
		return NULL;
	}

	if (r_buf_read_at (bin->b, *curAddr, (ut8*) varFileInfo->szKey, VARFILEINFO_UTF_16_LEN) != VARFILEINFO_UTF_16_LEN) {
		bprintf ("Warning: read (VarFileInfo szKey)\n");
		free_VarFileInfo (varFileInfo);
		return NULL;
	}
	*curAddr += VARFILEINFO_UTF_16_LEN;

	if (memcmp (varFileInfo->szKey, VARFILEINFO_UTF_16, VARFILEINFO_UTF_16_LEN)) {
		bprintf ("Warning: check (VarFileInfo szKey)\n");
		free_VarFileInfo (varFileInfo);
		return NULL;
	}
	align32 (*curAddr);
	while (startAddr + varFileInfo->wLength > *curAddr) {
		Var** tmp = (Var**) realloc (varFileInfo->Children, (varFileInfo->numOfChildren + 1) * sizeof(*varFileInfo->Children));
		if (!tmp) {
			bprintf ("Warning: realloc (VarFileInfo Children)\n");
			free_VarFileInfo (varFileInfo);
			return NULL;
		}
		varFileInfo->Children = tmp;
		if (!(varFileInfo->Children[varFileInfo->numOfChildren] = Pe_r_bin_pe_parse_var (bin, curAddr))) {
			bprintf ("Warning: bad parsing Var\n");
			free_VarFileInfo (varFileInfo);
			return NULL;
		}
		varFileInfo->numOfChildren++;
		align32 (*curAddr);
	}
	return varFileInfo;
}
