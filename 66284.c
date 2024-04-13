GF_Err asrt_dump(GF_Box *a, FILE * trace)
{
	u32 i;
	GF_AdobeSegmentRunTableBox *p = (GF_AdobeSegmentRunTableBox*)a;
	gf_isom_box_dump_start(a, "AdobeSegmentRunTableBox", trace);

	fprintf(trace, ">\n");

	for (i=0; i<p->quality_entry_count; i++) {
		char *str = (char*)gf_list_get(p->quality_segment_url_modifiers, i);
		fprintf(trace, "<QualityEntry>%s</QualityEntry>\n", str);
	}

	for (i=0; i<p->segment_run_entry_count; i++) {
		GF_AdobeSegmentRunEntry *sre = (GF_AdobeSegmentRunEntry *)gf_list_get(p->segment_run_entry_table, i);
		fprintf(trace, "<SegmentRunEntry FirstSegment=\"%u\" FragmentsPerSegment=\"%u\"/>\n", sre->first_segment, sre->fragment_per_segment);
	}

	gf_isom_box_dump_done("AdobeSegmentRunTableBox", a, trace);
	return GF_OK;
}
