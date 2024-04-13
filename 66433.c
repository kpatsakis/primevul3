GF_Err stri_dump(GF_Box *a, FILE * trace)
{
	u32 i;
	GF_SubTrackInformationBox *p = (GF_SubTrackInformationBox *)a;
	gf_isom_box_dump_start(a, "SubTrackInformationBox", trace);

	fprintf(trace, "switch_group=\"%d\" alternate_group=\"%d\" sub_track_id=\"%d\">\n", p->switch_group, p->alternate_group, p->sub_track_id);

	for (i = 0; i < p->attribute_count; i++) {
		fprintf(trace, "<SubTrackInformationAttribute value=\"%s\"/>\n", gf_4cc_to_str(p->attribute_list[i]) );
	}
	if (!p->size)
		fprintf(trace, "<SubTrackInformationAttribute value=\"\"/>\n");

	gf_isom_box_dump_done("SubTrackInformationBox", a, trace);
	return GF_OK;
}
