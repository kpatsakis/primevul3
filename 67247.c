static PE_DWord bin_pe_va_to_rva(RBinPEObj* bin, PE_DWord va) {
	ut64 imageBase = PE_(r_bin_pe_get_image_base) (bin);
	if (va < imageBase) {
		return va;
	}
	return va - imageBase;
}
