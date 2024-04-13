void MACH0_(mach_headerfields)(RBinFile *file) {
	RBuffer *buf = file->buf;
	int n = 0;
	struct MACH0_(mach_header) *mh = MACH0_(get_hdr_from_bytes)(buf);
	if (!mh) {
		return;
	}
	printf ("0x00000000  Magic       0x%x\n", mh->magic);
	printf ("0x00000004  CpuType     0x%x\n", mh->cputype);
	printf ("0x00000008  CpuSubType  0x%x\n", mh->cpusubtype);
	printf ("0x0000000c  FileType    0x%x\n", mh->filetype);
	printf ("0x00000010  nCmds       %d\n", mh->ncmds);
	printf ("0x00000014  sizeOfCmds  %d\n", mh->sizeofcmds);
	printf ("0x00000018  Flags       0x%x\n", mh->flags);

	ut64 addr = 0x20 - 4;
	ut32 word = 0;
	ut8 wordbuf[sizeof (word)];
#define READWORD() \
		addr += 4; \
		if (!r_buf_read_at (buf, addr, (ut8*)wordbuf, 4)) { \
			eprintf ("Invalid address in buffer."); \
			break; \
		} \
		word = r_read_le32 (wordbuf);
	for (n = 0; n < mh->ncmds; n++) {
		printf ("\n# Load Command %d\n", n);
		READWORD();
		int lcType = word;
		eprintf ("0x%08"PFMT64x"  cmd          0x%x %s\n",
			addr, lcType, cmd_to_string (lcType));
		READWORD();
		int lcSize = word;
		word &= 0xFFFFFF;
		printf ("0x%08"PFMT64x"  cmdsize      %d\n", addr, word);
		if (lcSize < 1) {
			eprintf ("Invalid size for a load command\n");
			break;
		}
		switch (lcType) {
		case LC_ID_DYLIB: // install_name_tool
			printf ("0x%08"PFMT64x"  id           %s\n",
				addr + 20, r_buf_get_at (buf, addr + 20, NULL));
			break;
		case LC_UUID:
			printf ("0x%08"PFMT64x"  uuid         %s\n",
				addr + 20, r_buf_get_at (buf, addr + 32, NULL));
			break;
		case LC_LOAD_DYLIB:
			printf ("0x%08"PFMT64x"  uuid         %s\n",
				addr + 20, r_buf_get_at (buf, addr + 20, NULL));
			break;
		case LC_RPATH:
			printf ("0x%08"PFMT64x"  uuid         %s\n",
				addr + 8, r_buf_get_at (buf, addr + 8, NULL));
			break;
		case LC_CODE_SIGNATURE:
			{
			ut32 *words = (ut32*)r_buf_get_at (buf, addr + 4, NULL);
			printf ("0x%08"PFMT64x"  dataoff      0x%08x\n", addr + 4, words[0]);
			printf ("0x%08"PFMT64x"  datasize     %d\n", addr + 8, words[1]);
			printf ("# wtf mach0.sign %d @ 0x%x\n", words[1], words[0]);
			}
			break;
		}
		addr += word - 8;
	}
	free (mh);
}
