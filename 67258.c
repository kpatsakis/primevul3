static void free_rsdr_hdr(SCV_RSDS_HEADER* rsds_hdr) {
	R_FREE (rsds_hdr->file_name);
}
