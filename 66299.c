GF_Err ctts_dump(GF_Box *a, FILE * trace)
{
	GF_CompositionOffsetBox *p;
	u32 i, nb_samples;
	p = (GF_CompositionOffsetBox *)a;
	gf_isom_box_dump_start(a, "CompositionOffsetBox", trace);
	fprintf(trace, "EntryCount=\"%d\">\n", p->nb_entries);

	nb_samples = 0;
	for (i=0; i<p->nb_entries; i++) {
		fprintf(trace, "<CompositionOffsetEntry CompositionOffset=\"%d\" SampleCount=\"%d\"/>\n", p->entries[i].decodingOffset, p->entries[i].sampleCount);
		nb_samples += p->entries[i].sampleCount;
	}
	if (p->size)
		fprintf(trace, "<!-- counted %d samples in CTTS entries -->\n", nb_samples);
	else
		fprintf(trace, "<CompositionOffsetEntry CompositionOffset=\"\" SampleCount=\"\"/>\n");

	gf_isom_box_dump_done("CompositionOffsetBox", a, trace);
	return GF_OK;
}
