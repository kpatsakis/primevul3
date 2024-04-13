struct r_bin_pe_addr_t *PE_(check_mingw) (struct PE_(r_bin_pe_obj_t) *bin) {
	struct r_bin_pe_addr_t* entry;
	int sw = 0;
	ut8 b[1024];
	int n = 0;
	if (!bin || !bin->b) {
		return 0LL;
	}
	entry = PE_(r_bin_pe_get_entrypoint) (bin);
	ZERO_FILL (b);
	if (r_buf_read_at (bin->b, entry->paddr, b, sizeof (b)) < 0) {
		bprintf ("Warning: Cannot read entry at 0x%08"PFMT64x "\n", entry->paddr);
		free (entry);
		return NULL;
	}
	if (b[0] == 0x55 && b[1] == 0x89 && b[3] == 0x83 && b[6] == 0xc7 && b[13] == 0xff && b[19] == 0xe8) {
		const st32 jmp_dst = (st32) r_read_le32 (&b[20]);
		entry->paddr += (5 + 19 + jmp_dst);
		entry->vaddr += (5 + 19 + jmp_dst);
		sw = 1;
	}
	if (b[0] == 0x83 && b[3] == 0xc7 && b[10] == 0xff && b[16] == 0xe8) {
		const st32 jmp_dst = (st32) r_read_le32 (&b[17]);
		entry->paddr += (5 + 16 + jmp_dst);
		entry->vaddr += (5 + 16 + jmp_dst);
		sw = 1;
	}
	if (b[0] == 0x83 && b[3] == 0xc7 && b[13] == 0xe8 && b[18] == 0x83 && b[21] == 0xe9) {
		const st32 jmp_dst = (st32) r_read_le32 (&b[22]);
		entry->paddr += (5 + 21 + jmp_dst);
		entry->vaddr += (5 + 21 + jmp_dst);
		sw = 1;
	}
	if (sw) {
		if (r_buf_read_at (bin->b, entry->paddr, b, sizeof (b)) > 0) {
			for (n = 0; n < sizeof (b) - 12; n++) {
				if (b[n] == 0xa1 && b[n + 5] == 0x89 && b[n + 8] == 0xe8) {
					const st32 call_dst = (st32) r_read_le32 (&b[n + 9]);
					entry->paddr += (n + 5 + 8 + call_dst);
					entry->vaddr += (n + 5 + 8 + call_dst);
					return entry;
				}
			}
		}
	}
	free (entry);
	return NULL;
}
