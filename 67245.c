static PE_DWord bin_pe_rva_to_va(RBinPEObj* bin, PE_DWord rva) {
	return PE_(r_bin_pe_get_image_base) (bin) + rva;
}
