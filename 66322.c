GF_Err fdpa_dump(GF_Box *a, FILE * trace)
{
	u32 i;
	GF_FDpacketBox *ptr = (GF_FDpacketBox *) a;
	if (!a) return GF_BAD_PARAM;

	gf_isom_box_dump_start(a, "FDpacketBox", trace);
	fprintf(trace, "sender_current_time_present=\"%d\" expected_residual_time_present=\"%d\" session_close_bit=\"%d\" object_close_bit=\"%d\" transport_object_identifier=\"%d\">\n", ptr->info.sender_current_time_present, ptr->info.expected_residual_time_present, ptr->info.session_close_bit, ptr->info.object_close_bit, ptr->info.transport_object_identifier);

	for (i=0; i<ptr->header_ext_count; i++) {
		fprintf(trace, "<FDHeaderExt type=\"%d\"", ptr->headers[i].header_extension_type);
		if (ptr->headers[i].header_extension_type > 127) {
			dump_data_attribute(trace, "content", (char *) ptr->headers[i].content, 3);
		} else if (ptr->headers[i].data_length) {
			dump_data_attribute(trace, "data", ptr->headers[i].data, ptr->headers[i].data_length);
		}
		fprintf(trace, "/>\n");
	}
	if (!ptr->size) {
		fprintf(trace, "<FDHeaderExt type=\"\" content=\"\" data=\"\"/>\n");
	}
	gf_isom_box_dump_done("FDpacketBox", a, trace);
	return GF_OK;
}
