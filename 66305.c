GF_Err dims_dump(GF_Box *a, FILE * trace)
{
	GF_DIMSSampleEntryBox *p = (GF_DIMSSampleEntryBox*)a;
	gf_isom_box_dump_start(a, "DIMSSampleEntryBox", trace);
	fprintf(trace, "dataReferenceIndex=\"%d\">\n", p->dataReferenceIndex);
	if (p->config) gf_isom_box_dump(p->config, trace);
	if (p->scripts) gf_isom_box_dump(p->scripts, trace);
	gf_isom_box_array_dump(p->protections, trace);
	gf_isom_box_dump_done("DIMSSampleEntryBox", a, trace);
	return GF_OK;
}
