GF_Err stsc_dump(GF_Box *a, FILE * trace)
{
	GF_SampleToChunkBox *p;
	u32 i, nb_samples;

	p = (GF_SampleToChunkBox *)a;
	gf_isom_box_dump_start(a, "SampleToChunkBox", trace);
	fprintf(trace, "EntryCount=\"%d\">\n", p->nb_entries);

	nb_samples = 0;
	for (i=0; i<p->nb_entries; i++) {
		fprintf(trace, "<SampleToChunkEntry FirstChunk=\"%d\" SamplesPerChunk=\"%d\" SampleDescriptionIndex=\"%d\"/>\n", p->entries[i].firstChunk, p->entries[i].samplesPerChunk, p->entries[i].sampleDescriptionIndex);
		if (i+1<p->nb_entries) {
			nb_samples += (p->entries[i+1].firstChunk - p->entries[i].firstChunk) * p->entries[i].samplesPerChunk;
		} else {
			nb_samples += p->entries[i].samplesPerChunk;
		}
	}
	if (p->size)
		fprintf(trace, "<!-- counted %d samples in STSC entries (could be less than sample count) -->\n", nb_samples);
	else
		fprintf(trace, "<SampleToChunkEntry FirstChunk=\"\" SamplesPerChunk=\"\" SampleDescriptionIndex=\"\"/>\n");

	gf_isom_box_dump_done("SampleToChunkBox", a, trace);
	return GF_OK;
}
