static int bin_pe_init(struct PE_(r_bin_pe_obj_t)* bin) {
	bin->dos_header = NULL;
	bin->nt_headers = NULL;
	bin->section_header = NULL;
	bin->export_directory = NULL;
	bin->import_directory = NULL;
	bin->resource_directory = NULL;
	bin->delay_import_directory = NULL;
	bin->optional_header = NULL;
	bin->data_directory = NULL;
	bin->big_endian = 0;
	if (!bin_pe_init_hdr (bin)) {
		eprintf ("Warning: File is not PE\n");
		return false;
	}
	if (!bin_pe_init_sections (bin)) {
		eprintf ("Warning: Cannot initialize sections\n");
		return false;
	}
	bin_pe_init_imports (bin);
	bin_pe_init_exports (bin);
	bin_pe_init_resource (bin);
	bin_pe_get_certificate(bin);

	bin->big_endian = PE_(r_bin_pe_is_big_endian) (bin);

	bin_pe_init_tls (bin);
	bin_pe_init_clr_hdr (bin);
	bin_pe_init_metadata_hdr (bin);
	bin_pe_init_overlay (bin);
	PE_(bin_pe_parse_resource) (bin);
	bin->relocs = NULL;
	return true;
}
