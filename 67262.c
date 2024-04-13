static inline int is_arm(struct PE_(r_bin_pe_obj_t)* bin) {
	switch (bin->nt_headers->file_header.Machine) {
	case PE_IMAGE_FILE_MACHINE_RPI2: // 462
	case PE_IMAGE_FILE_MACHINE_ARM:
	case PE_IMAGE_FILE_MACHINE_THUMB:
		return 1;
	}
	return 0;
}
