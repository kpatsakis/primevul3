static int bin_pe_init_exports(struct PE_(r_bin_pe_obj_t)* bin) {
	PE_(image_data_directory) * data_dir_export = &bin->data_directory[PE_IMAGE_DIRECTORY_ENTRY_EXPORT];
	PE_DWord export_dir_paddr = bin_pe_rva_to_paddr (bin, data_dir_export->VirtualAddress);
	if (!export_dir_paddr) {
		return false;
	}
	if (!(bin->export_directory = malloc (sizeof(PE_(image_export_directory))))) {
		r_sys_perror ("malloc (export directory)");
		return false;
	}
	if (r_buf_read_at (bin->b, export_dir_paddr, (ut8*) bin->export_directory, sizeof (PE_(image_export_directory))) == -1) {
		bprintf ("Warning: read (export directory)\n");
		free (bin->export_directory);
		bin->export_directory = NULL;
		return false;
	}
	return true;
}
