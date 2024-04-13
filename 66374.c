GF_Err lsr1_dump(GF_Box *a, FILE * trace)
{
	GF_LASeRSampleEntryBox *p = (GF_LASeRSampleEntryBox*)a;
	gf_isom_box_dump_start(a, "LASeRSampleEntryBox", trace);
	fprintf(trace, "DataReferenceIndex=\"%d\">\n", p->dataReferenceIndex);
	if (p->lsr_config) gf_isom_box_dump(p->lsr_config, trace);
	if (p->descr) gf_isom_box_dump(p->descr, trace);
	gf_isom_box_dump_done("LASeRSampleEntryBox", a, trace);
	return GF_OK;
}
