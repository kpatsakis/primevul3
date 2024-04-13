GF_Err stsg_dump(GF_Box *a, FILE * trace)
{
	u32 i;
	GF_SubTrackSampleGroupBox *p = (GF_SubTrackSampleGroupBox *)a;
	gf_isom_box_dump_start(a, "SubTrackSampleGroupBox", trace);

	if (p->grouping_type)
		fprintf(trace, "grouping_type=\"%s\"", gf_4cc_to_str(p->grouping_type) );
	fprintf(trace, ">\n");

	for (i = 0; i < p->nb_groups; i++) {
		fprintf(trace, "<SubTrackSampleGroupBoxEntry group_description_index=\"%d\"/>\n", p->group_description_index[i]);
	}
	if (!p->size)
		fprintf(trace, "<SubTrackSampleGroupBoxEntry group_description_index=\"\"/>\n");

	gf_isom_box_dump_done("SubTrackSampleGroupBox", a, trace);
	return GF_OK;
}
