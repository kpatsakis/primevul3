static void init_rsdr_hdr(SCV_RSDS_HEADER* rsds_hdr) {
	memset (rsds_hdr, 0, sizeof (SCV_RSDS_HEADER));
	rsds_hdr->free = (void (*)(struct SCV_RSDS_HEADER*))free_rsdr_hdr;
}
