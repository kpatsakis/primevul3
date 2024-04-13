GF_Err stss_dump(GF_Box *a, FILE * trace)
{
	GF_SyncSampleBox *p;
	u32 i;

	p = (GF_SyncSampleBox *)a;
	gf_isom_box_dump_start(a, "SyncSampleBox", trace);
	fprintf(trace, "EntryCount=\"%d\">\n", p->nb_entries);

	if (!p->sampleNumbers && p->size) {
		fprintf(trace, "<!--Warning: No Key Frames indications-->\n");
	} else {
		for (i=0; i<p->nb_entries; i++) {
			fprintf(trace, "<SyncSampleEntry sampleNumber=\"%u\"/>\n", p->sampleNumbers[i]);
		}
	}
	if (!p->size) {
			fprintf(trace, "<SyncSampleEntry sampleNumber=\"\"/>\n");
	}
	gf_isom_box_dump_done("SyncSampleBox", a, trace);
	return GF_OK;
}
