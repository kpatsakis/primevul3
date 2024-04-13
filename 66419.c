GF_Err saiz_dump(GF_Box *a, FILE * trace)
{
	u32 i;
	GF_SampleAuxiliaryInfoSizeBox *ptr = (GF_SampleAuxiliaryInfoSizeBox*) a;
	if (!a) return GF_BAD_PARAM;

	gf_isom_box_dump_start(a, "SampleAuxiliaryInfoSizeBox", trace);

	fprintf(trace, "default_sample_info_size=\"%d\" sample_count=\"%d\"", ptr->default_sample_info_size, ptr->sample_count);
	if (ptr->flags & 1) {
		if (isalnum(ptr->aux_info_type>>24)) {
			fprintf(trace, " aux_info_type=\"%s\" aux_info_type_parameter=\"%d\"", gf_4cc_to_str(ptr->aux_info_type), ptr->aux_info_type_parameter);
		} else {
			fprintf(trace, " aux_info_type=\"%d\" aux_info_type_parameter=\"%d\"", ptr->aux_info_type, ptr->aux_info_type_parameter);
		}
	}
	fprintf(trace, ">\n");
	if (ptr->default_sample_info_size==0) {
		for (i=0; i<ptr->sample_count; i++) {
			fprintf(trace, "<SAISize size=\"%d\" />\n", ptr->sample_info_size[i]);
		}
	}
	if (!ptr->size) {
			fprintf(trace, "<SAISize size=\"\" />\n");
	}
	gf_isom_box_dump_done("SampleAuxiliaryInfoSizeBox", a, trace);
	return GF_OK;
}
