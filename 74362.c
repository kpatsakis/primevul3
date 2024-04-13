static int bin_versioninfo(RCore *r, int mode) {
	const RBinInfo *info = r_bin_get_info (r->bin);
	if (!info || !info->rclass) {
		return false;
	}
	if (!strncmp ("pe", info->rclass, 2)) {
		bin_pe_versioninfo (r, mode);
	} else if (!strncmp ("elf", info->rclass, 3)) {
		bin_elf_versioninfo (r, mode);
	} else if (!strncmp ("mach0", info->rclass, 5)) {
		bin_mach0_versioninfo (r);
	} else {
		r_cons_println ("Unknown format");
		return false;
	}
	return true;
}
