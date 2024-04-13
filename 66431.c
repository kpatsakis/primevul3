GF_Err ssix_dump(GF_Box *a, FILE * trace)
{
	u32 i, j;
	GF_SubsegmentIndexBox *p = (GF_SubsegmentIndexBox *)a;
	gf_isom_box_dump_start(a, "SubsegmentIndexBox", trace);

	fprintf(trace, "subsegment_count=\"%d\" >\n", p->subsegment_count);
	for (i = 0; i < p->subsegment_count; i++) {
		fprintf(trace, "<Subsegment range_count=\"%d\">\n", p->subsegments[i].range_count);
		for (j = 0; j < p->subsegments[i].range_count; j++) {
			fprintf(trace, "<Range level=\"%d\" range_size=\"%d\"/>\n", p->subsegments[i].levels[j], p->subsegments[i].range_sizes[j]);
		}
		fprintf(trace, "</Subsegment>\n");
	}
	if (!p->size) {
		fprintf(trace, "<Subsegment range_count=\"\">\n");
		fprintf(trace, "<Range level=\"\" range_size=\"\"/>\n");
		fprintf(trace, "</Subsegment>\n");
	}
	gf_isom_box_dump_done("SubsegmentIndexBox", a, trace);
	return GF_OK;
}
