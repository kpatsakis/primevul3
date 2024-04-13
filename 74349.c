static int bin_main(RCore *r, int mode, int va) {
	RBinAddr *binmain = r_bin_get_sym (r->bin, R_BIN_SYM_MAIN);
	ut64 addr;
	if (!binmain) {
		return false;
	}
	addr = va ? r_bin_a2b (r->bin, binmain->vaddr) : binmain->paddr;

	if (IS_MODE_SET (mode)) {
		r_flag_space_set (r->flags, R_FLAGS_FS_SYMBOLS);
		r_flag_set (r->flags, "main", addr, r->blocksize);
	} else if (IS_MODE_SIMPLE (mode)) {
		r_cons_printf ("%"PFMT64d, addr);
	} else if (IS_MODE_RAD (mode)) {
		r_cons_printf ("fs symbols\n");
		r_cons_printf ("f main @ 0x%08"PFMT64x"\n", addr);
	} else if (IS_MODE_JSON (mode)) {
		r_cons_printf ("{\"vaddr\":%" PFMT64d
			",\"paddr\":%" PFMT64d "}", addr, binmain->paddr);
	} else {
		r_cons_printf ("[Main]\n");
		r_cons_printf ("vaddr=0x%08"PFMT64x" paddr=0x%08"PFMT64x"\n",
			addr, binmain->paddr);
	}
	return true;
}
