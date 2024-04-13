static void bin_pe_get_certificate(struct PE_ (r_bin_pe_obj_t) * bin) {
	ut64 size, vaddr;
	ut8 *data = NULL;
	int len;
	if (!bin || !bin->nt_headers) {
		return;
	}
	bin->cms = NULL;
	size = bin->data_directory[PE_IMAGE_DIRECTORY_ENTRY_SECURITY].Size;
	vaddr = bin->data_directory[PE_IMAGE_DIRECTORY_ENTRY_SECURITY].VirtualAddress;
	data = calloc (1, size);
	if (!data) {
		return;
	}
	if (vaddr > bin->size || vaddr + size > bin->size) {
		bprintf ("vaddr greater than the file\n");
		free (data);
		return;
	}
	len = r_buf_read_at (bin->b, vaddr + 8, data, size - 8);
	if (len < 1) {
		R_FREE (data);
		return;
	}
	bin->cms = r_pkcs7_parse_cms (data, size);
	bin->is_signed = bin->cms != NULL;
	R_FREE (data);
}
