GF_Err afrt_dump(GF_Box *a, FILE * trace)
{
	u32 i;
	GF_AdobeFragmentRunTableBox *p = (GF_AdobeFragmentRunTableBox*)a;
	gf_isom_box_dump_start(a, "AdobeFragmentRunTableBox", trace);

	fprintf(trace, "TimeScale=\"%u\">\n", p->timescale);

	for (i=0; i<p->quality_entry_count; i++) {
		char *str = (char*)gf_list_get(p->quality_segment_url_modifiers, i);
		fprintf(trace, "<QualityEntry>%s</QualityEntry>\n", str);
	}

	for (i=0; i<p->fragment_run_entry_count; i++) {
		GF_AdobeFragmentRunEntry *fre = (GF_AdobeFragmentRunEntry *)gf_list_get(p->fragment_run_entry_table, i);
		fprintf(trace, "<FragmentRunEntry FirstFragment=\"%u\" FirstFragmentTimestamp=\""LLU"\" FirstFragmentDuration=\"%u\"", fre->first_fragment, fre->first_fragment_timestamp, fre->fragment_duration);
		if (!fre->fragment_duration)
			fprintf(trace, " DiscontinuityIndicator=\"%u\"", fre->discontinuity_indicator);
		fprintf(trace, "/>\n");
	}

	gf_isom_box_dump_done("AdobeFragmentRunTableBox", a, trace);
	return GF_OK;
}
