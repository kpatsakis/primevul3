static PE_VS_VERSIONINFO* Pe_r_bin_pe_parse_version_info(struct PE_(r_bin_pe_obj_t)* bin, PE_DWord version_info_paddr) {
	ut32 sz;
	PE_VS_VERSIONINFO* vs_VersionInfo = calloc (1, sizeof(PE_VS_VERSIONINFO));
	if (!vs_VersionInfo) {
		return NULL;
	}
	PE_DWord startAddr = version_info_paddr;
	PE_DWord curAddr = version_info_paddr;
	sz = sizeof(ut16);
	EXIT_ON_OVERFLOW (sz);
	if (r_buf_read_at (bin->b, curAddr, (ut8*) &vs_VersionInfo->wLength, sz) != sz) {
		bprintf ("Warning: read (VS_VERSIONINFO wLength)\n");
		goto out_error;
	}
	curAddr += sz;
	EXIT_ON_OVERFLOW (sz);
	if (r_buf_read_at (bin->b, curAddr, (ut8*) &vs_VersionInfo->wValueLength, sz) != sz) {
		bprintf ("Warning: read (VS_VERSIONINFO wValueLength)\n");
		goto out_error;
	}
	curAddr += sz;
	EXIT_ON_OVERFLOW (sz);
	if (r_buf_read_at (bin->b, curAddr, (ut8*) &vs_VersionInfo->wType, sz) != sz) {
		bprintf ("Warning: read (VS_VERSIONINFO wType)\n");
		goto out_error;
	}
	curAddr += sz;
	if (vs_VersionInfo->wType && vs_VersionInfo->wType != 1) {
		bprintf ("Warning: check (VS_VERSIONINFO wType)\n");
		goto out_error;
	}

	vs_VersionInfo->szKey = (ut16*) malloc (UT16_ALIGN (VS_VERSION_INFO_UTF_16_LEN));  //L"VS_VERSION_INFO"
	if (!vs_VersionInfo->szKey) {
		bprintf ("Warning: malloc (VS_VERSIONINFO szKey)\n");
		goto out_error;
	}
	sz = VS_VERSION_INFO_UTF_16_LEN;
	EXIT_ON_OVERFLOW (sz);
	if (r_buf_read_at (bin->b, curAddr, (ut8*) vs_VersionInfo->szKey, sz) != sz) {
		bprintf ("Warning: read (VS_VERSIONINFO szKey)\n");
		goto out_error;
	}
	curAddr += sz;
	if (memcmp (vs_VersionInfo->szKey, VS_VERSION_INFO_UTF_16, sz)) {
		goto out_error;
	}
	align32 (curAddr);
	if (vs_VersionInfo->wValueLength) {
		if (vs_VersionInfo->wValueLength != sizeof (*vs_VersionInfo->Value)) {
			bprintf ("Warning: check (VS_VERSIONINFO wValueLength != sizeof PE_VS_FIXEDFILEINFO)\n");
			goto out_error;
		}

		vs_VersionInfo->Value = (PE_VS_FIXEDFILEINFO*) malloc (sizeof(*vs_VersionInfo->Value));
		if (!vs_VersionInfo->Value) {
			bprintf ("Warning: malloc (VS_VERSIONINFO Value)\n");
			goto out_error;
		}
		sz = sizeof(PE_VS_FIXEDFILEINFO);
		EXIT_ON_OVERFLOW (sz);
		if (r_buf_read_at (bin->b, curAddr, (ut8*) vs_VersionInfo->Value, sz) != sz) {
			bprintf ("Warning: read (VS_VERSIONINFO Value)\n");
			goto out_error;
		}

		if (vs_VersionInfo->Value->dwSignature != 0xFEEF04BD) {
			bprintf ("Warning: check (PE_VS_FIXEDFILEINFO signature) 0x%08x\n", vs_VersionInfo->Value->dwSignature);
			goto out_error;
		}
		curAddr += sz;
		align32 (curAddr);
	}

	if (startAddr + vs_VersionInfo->wLength > curAddr) {
		char t = '\0';
		if (curAddr + 3 * sizeof(ut16) > bin->size || curAddr + 3 + sizeof(ut64) + 1 > bin->size) {
			goto out_error;
		}
		if (r_buf_read_at (bin->b, curAddr + 3 * sizeof(ut16), (ut8*) &t, 1) != 1) {
			bprintf ("Warning: read (VS_VERSIONINFO Children V or S)\n");
			goto out_error;
		}
		if (!(t == 'S' || t == 'V')) {
			bprintf ("Warning: bad type (VS_VERSIONINFO Children)\n");
			goto out_error;
		}
		if (t == 'S') {
			if (!(vs_VersionInfo->stringFileInfo = Pe_r_bin_pe_parse_string_file_info (bin, &curAddr))) {
				bprintf ("Warning: bad parsing (VS_VERSIONINFO StringFileInfo)\n");
				goto out_error;
			}
		}
		if (t == 'V') {
			if (!(vs_VersionInfo->varFileInfo = Pe_r_bin_pe_parse_var_file_info (bin, &curAddr))) {
				bprintf ("Warning: bad parsing (VS_VERSIONINFO VarFileInfo)\n");
				goto out_error;
			}
		}

		align32 (curAddr);

		if (startAddr + vs_VersionInfo->wLength > curAddr) {
			if (t == 'V') {
				if (!(vs_VersionInfo->stringFileInfo = Pe_r_bin_pe_parse_string_file_info (bin, &curAddr))) {
					bprintf ("Warning: bad parsing (VS_VERSIONINFO StringFileInfo)\n");
					goto out_error;
				}
			} else if (t == 'S') {
				if (!(vs_VersionInfo->varFileInfo = Pe_r_bin_pe_parse_var_file_info (bin, &curAddr))) {
					bprintf ("Warning: bad parsing (VS_VERSIONINFO VarFileInfo)\n");
					goto out_error;
				}
			}
			if (startAddr + vs_VersionInfo->wLength > curAddr) {
				bprintf ("Warning: bad parsing (VS_VERSIONINFO wLength left)\n");
				goto out_error;
			}
		}
	}
	return vs_VersionInfo;
out_error:
	free_VS_VERSIONINFO (vs_VersionInfo);
	return NULL;

}
