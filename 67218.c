static StringFileInfo* Pe_r_bin_pe_parse_string_file_info(struct PE_(r_bin_pe_obj_t)* bin, PE_DWord* curAddr) {
	StringFileInfo* stringFileInfo = calloc (1, sizeof(*stringFileInfo));
	if (!stringFileInfo) {
		bprintf ("Warning: calloc (StringFileInfo)\n");
		return NULL;
	}

	PE_DWord startAddr = *curAddr;

	if (r_buf_read_at (bin->b, *curAddr, (ut8*) &stringFileInfo->wLength, sizeof(stringFileInfo->wLength)) != sizeof(stringFileInfo->wLength)) {
		bprintf ("Warning: read (StringFileInfo wLength)\n");
		free_StringFileInfo (stringFileInfo);
		return NULL;
	}
	*curAddr += sizeof(stringFileInfo->wLength);

	if (r_buf_read_at (bin->b, *curAddr, (ut8*) &stringFileInfo->wValueLength, sizeof(stringFileInfo->wValueLength)) != sizeof(stringFileInfo->wValueLength)) {
		bprintf ("Warning: read (StringFileInfo wValueLength)\n");
		free_StringFileInfo (stringFileInfo);
		return NULL;
	}
	*curAddr += sizeof(stringFileInfo->wValueLength);

	if (stringFileInfo->wValueLength) {
		bprintf ("Warning: check (StringFileInfo wValueLength)\n");
		free_StringFileInfo (stringFileInfo);
		return NULL;
	}

	if (r_buf_read_at (bin->b, *curAddr, (ut8*) &stringFileInfo->wType, sizeof(stringFileInfo->wType)) != sizeof(stringFileInfo->wType)) {
		bprintf ("Warning: read (StringFileInfo wType)\n");
		free_StringFileInfo (stringFileInfo);
		return NULL;
	}
	*curAddr += sizeof(stringFileInfo->wType);

	if (stringFileInfo->wType && stringFileInfo->wType != 1) {
		bprintf ("Warning: check (StringFileInfo wType)\n");
		free_StringFileInfo (stringFileInfo);
		return NULL;
	}

	stringFileInfo->szKey = (ut16*) malloc (UT16_ALIGN (STRINGFILEINFO_UTF_16_LEN));  //L"StringFileInfo"
	if (!stringFileInfo->szKey) {
		bprintf ("Warning: malloc (StringFileInfo szKey)\n");
		free_StringFileInfo (stringFileInfo);
		return NULL;
	}

	if (r_buf_read_at (bin->b, *curAddr, (ut8*) stringFileInfo->szKey, STRINGFILEINFO_UTF_16_LEN) != STRINGFILEINFO_UTF_16_LEN) {
		bprintf ("Warning: read (StringFileInfo szKey)\n");
		free_StringFileInfo (stringFileInfo);
		return NULL;
	}
	*curAddr += STRINGFILEINFO_UTF_16_LEN;

	if (memcmp (stringFileInfo->szKey, STRINGFILEINFO_UTF_16, STRINGFILEINFO_UTF_16_LEN) != 0) {
		bprintf ("Warning: check (StringFileInfo szKey)\n");
		free_StringFileInfo (stringFileInfo);
		return NULL;
	}

	align32 (*curAddr);

	while (startAddr + stringFileInfo->wLength > *curAddr) {
		StringTable** tmp = (StringTable**) realloc (stringFileInfo->Children, (stringFileInfo->numOfChildren + 1) * sizeof(*stringFileInfo->Children));
		if (!tmp) {
			bprintf ("Warning: realloc (StringFileInfo Children)\n");
			free_StringFileInfo (stringFileInfo);
			return NULL;
		}
		stringFileInfo->Children = tmp;
		if (!(stringFileInfo->Children[stringFileInfo->numOfChildren] = Pe_r_bin_pe_parse_string_table (bin, curAddr))) {
			bprintf ("Warning: bad parsing StringTable\n");
			free_StringFileInfo (stringFileInfo);
			return NULL;
		}
		stringFileInfo->numOfChildren++;
		align32 (*curAddr);
	}

	if (!stringFileInfo->numOfChildren) {
		bprintf ("Warning: check (StringFileInfo numOfChildren)\n");
		free_StringFileInfo (stringFileInfo);
		return NULL;
	}

	return stringFileInfo;
}
