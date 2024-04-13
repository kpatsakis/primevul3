GF_Err saio_dump(GF_Box *a, FILE * trace)
{
	u32 i;
	GF_SampleAuxiliaryInfoOffsetBox *ptr = (GF_SampleAuxiliaryInfoOffsetBox*) a;
	if (!a) return GF_BAD_PARAM;

	gf_isom_box_dump_start(a, "SampleAuxiliaryInfoOffsetBox", trace);

	fprintf(trace, "entry_count=\"%d\"", ptr->entry_count);
	if (ptr->flags & 1) {
		if (isalnum(ptr->aux_info_type>>24)) {
			fprintf(trace, " aux_info_type=\"%s\" aux_info_type_parameter=\"%d\"", gf_4cc_to_str(ptr->aux_info_type), ptr->aux_info_type_parameter);
		} else {
			fprintf(trace, " aux_info_type=\"%d\" aux_info_type_parameter=\"%d\"", ptr->aux_info_type, ptr->aux_info_type_parameter);
		}
	}

	fprintf(trace, ">\n");

	if (ptr->version==0) {
		for (i=0; i<ptr->entry_count; i++) {
			fprintf(trace, "<SAIChunkOffset offset=\"%d\"/>\n", ptr->offsets[i]);
		}
	} else {
		for (i=0; i<ptr->entry_count; i++) {
			fprintf(trace, "<SAIChunkOffset offset=\""LLD"\"/>\n", ptr->offsets_large[i]);
		}
	}
	if (!ptr->size) {
			fprintf(trace, "<SAIChunkOffset offset=\"\"/>\n");
	}
	gf_isom_box_dump_done("SampleAuxiliaryInfoOffsetBox", a, trace);
	return GF_OK;
}
