RList* MACH0_(mach_fields)(RBinFile *bf) {
	struct MACH0_(mach_header) *mh = MACH0_(get_hdr_from_bytes)(bf->buf);
	if (!mh) {
		return NULL;
	}
	RList *ret = r_list_new ();
	if (!ret) {
		return NULL;
	}
	ret->free = free;
	ut64 addr = 0;

#define ROW(nam,siz,val,fmt) \
	r_list_append (ret, r_bin_field_new (addr, addr, siz, nam, sdb_fmt ("0x%08x", val), fmt)); \
	addr += 4;
	ROW("hdr.magic", 4, mh->magic, "x");
	ROW("hdr.cputype", 4, mh->cputype, NULL);
	ROW("hdr.cpusubtype", 4, mh->cpusubtype, NULL);
	ROW("hdr.filetype", 4, mh->filetype, NULL);
	ROW("hdr.ncmds", 4, mh->ncmds, NULL);
	ROW("hdr.sizeofcmds", 4, mh->sizeofcmds, NULL);
	free (mh);
	return ret;
}
