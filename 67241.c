static int bin_pe_init_resource(struct PE_(r_bin_pe_obj_t)* bin) {
	PE_(image_data_directory) * resource_dir = &bin->data_directory[PE_IMAGE_DIRECTORY_ENTRY_RESOURCE];
	PE_DWord resource_dir_paddr = bin_pe_rva_to_paddr (bin, resource_dir->VirtualAddress);
	if (!resource_dir_paddr) {
		return false;
	}

	bin->resources = r_list_newf ((RListFree)_free_resources);
	if (!bin->resources) {
		return false;
	}
	if (!(bin->resource_directory = malloc (sizeof(*bin->resource_directory)))) {
		r_sys_perror ("malloc (resource directory)");
		return false;
	}
	if (r_buf_read_at (bin->b, resource_dir_paddr, (ut8*) bin->resource_directory,
		sizeof (*bin->resource_directory)) != sizeof (*bin->resource_directory)) {
		bprintf ("Warning: read (resource directory)\n");
		free (bin->resource_directory);
		bin->resource_directory = NULL;
		return false;
	}
	bin->resource_directory_offset = resource_dir_paddr;
	return true;
}
