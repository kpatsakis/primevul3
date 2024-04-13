GF_Err sbgp_dump(GF_Box *a, FILE * trace)
{
	u32 i;
	GF_SampleGroupBox *ptr = (GF_SampleGroupBox*) a;
	if (!a) return GF_BAD_PARAM;

	gf_isom_box_dump_start(a, "SampleGroupBox", trace);

	if (ptr->grouping_type)
		fprintf(trace, "grouping_type=\"%s\"", gf_4cc_to_str(ptr->grouping_type) );

	if (ptr->version==1) {
		if (isalnum(ptr->grouping_type_parameter&0xFF)) {
			fprintf(trace, " grouping_type_parameter=\"%s\"", gf_4cc_to_str(ptr->grouping_type_parameter) );
		} else {
			fprintf(trace, " grouping_type_parameter=\"%d\"", ptr->grouping_type_parameter);
		}
	}
	fprintf(trace, ">\n");
	for (i=0; i<ptr->entry_count; i++) {
		fprintf(trace, "<SampleGroupBoxEntry sample_count=\"%d\" group_description_index=\"%d\"/>\n", ptr->sample_entries[i].sample_count, ptr->sample_entries[i].group_description_index );
	}
	if (!ptr->size) {
		fprintf(trace, "<SampleGroupBoxEntry sample_count=\"\" group_description_index=\"\"/>\n");
	}
	gf_isom_box_dump_done("SampleGroupBox", a, trace);
	return GF_OK;
}
