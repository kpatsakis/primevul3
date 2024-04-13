GF_Err leva_dump(GF_Box *a, FILE * trace)
{
	u32 i;
	GF_LevelAssignmentBox *p = (GF_LevelAssignmentBox *)a;
	gf_isom_box_dump_start(a, "LevelAssignmentBox", trace);

	fprintf(trace, "level_count=\"%d\" >\n", p->level_count);
	for (i = 0; i < p->level_count; i++) {
		fprintf(trace, "<Assignement track_id=\"%d\" padding_flag=\"%d\" assignement_type=\"%d\" grouping_type=\"%s\" grouping_type_parameter=\"%d\" sub_track_id=\"%d\" />\n", p->levels[i].track_id, p->levels[i].padding_flag, p->levels[i].type, gf_4cc_to_str(p->levels[i].grouping_type) , p->levels[i].grouping_type_parameter, p->levels[i].sub_track_id);
	}
	if (!p->size) {
		fprintf(trace, "<Assignement track_id=\"\" padding_flag=\"\" assignement_type=\"\" grouping_type=\"\" grouping_type_parameter=\"\" sub_track_id=\"\" />\n");
	}
	gf_isom_box_dump_done("LevelAssignmentBox", a, trace);
	return GF_OK;
}
