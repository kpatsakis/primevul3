static int bin_pe_init_clr_hdr(struct PE_(r_bin_pe_obj_t)* bin) {
	PE_(image_data_directory) * clr_dir = &bin->data_directory[PE_IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR];
	PE_DWord image_clr_hdr_paddr = bin_pe_rva_to_paddr (bin, clr_dir->VirtualAddress);
	PE_(image_clr_header) * clr_hdr = R_NEW0 (PE_(image_clr_header));
	int rr, len = sizeof (PE_(image_clr_header));

	if (!clr_hdr) {
		return 0;
	}
	rr = r_buf_read_at (bin->b, image_clr_hdr_paddr, (ut8*) (clr_hdr), len);


	if (clr_hdr->HeaderSize != 0x48) {
		free (clr_hdr);
		return 0;
	}
	if (rr != len) {
		free (clr_hdr);
		return 0;
	}

	bin->clr_hdr = clr_hdr;
	return 1;
}
