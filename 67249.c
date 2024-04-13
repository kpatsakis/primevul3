struct r_bin_pe_addr_t *PE_(check_unknow) (struct PE_(r_bin_pe_obj_t) *bin) {
	struct r_bin_pe_addr_t *entry;
	if (!bin || !bin->b) {
		return 0LL;
	}
	ut8 *b = calloc (1, 512);
	if (!b) {
		return NULL;
	}
	entry = PE_ (r_bin_pe_get_entrypoint) (bin);
	if (r_buf_read_at (bin->b, entry->paddr, b, 512) < 1) {
		bprintf ("Warning: Cannot read entry at 0x%08"PFMT64x"\n", entry->paddr);
		free (entry);
		free (b);
		return NULL;
	}
	/* Decode the jmp instruction, this gets the address of the 'main'
	   function for PE produced by a compiler whose name someone forgot to
	   write down. */
	if (b[367] == 0xe8) {
		const st32 jmp_dst = (st32) r_read_le32 (&b[368]);
		entry->paddr += 367 + 5 + jmp_dst;
		entry->vaddr += 367 + 5 + jmp_dst;
		free (b);
		return entry;
	}
	int i;
	for (i = 0; i < 512 - 16 ; i++) {
		if (!memcmp (b + i, "\xff\x15", 2)) {
			if (b[i+6] == 0x50) {
				if (b[i+7] == 0xe8) {
					const st32 call_dst = (st32) r_read_le32 (&b[i + 8]);
					entry->paddr = entry->vaddr - entry->paddr;
					entry->vaddr += (i + 7 + 5 + (long)call_dst);
					entry->paddr += entry->vaddr;
					free (b);
					return entry;
				}
			}
		}
	}
	free (entry);
	free (b);
	return NULL;
}
