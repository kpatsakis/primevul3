GF_Err stts_dump(GF_Box *a, FILE * trace)
{
	GF_TimeToSampleBox *p;
	u32 i, nb_samples;

	p = (GF_TimeToSampleBox *)a;
	gf_isom_box_dump_start(a, "TimeToSampleBox", trace);
	fprintf(trace, "EntryCount=\"%d\">\n", p->nb_entries);

	nb_samples = 0;
	for (i=0; i<p->nb_entries; i++) {
		fprintf(trace, "<TimeToSampleEntry SampleDelta=\"%d\" SampleCount=\"%d\"/>\n", p->entries[i].sampleDelta, p->entries[i].sampleCount);
		nb_samples += p->entries[i].sampleCount;
	}
	if (p->size)
		fprintf(trace, "<!-- counted %d samples in STTS entries -->\n", nb_samples);
	else
		fprintf(trace, "<TimeToSampleEntry SampleDelta=\"\" SampleCount=\"\"/>\n");

	gf_isom_box_dump_done("TimeToSampleBox", a, trace);
	return GF_OK;
}
