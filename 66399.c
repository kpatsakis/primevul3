GF_Err padb_dump(GF_Box *a, FILE * trace)
{
	GF_PaddingBitsBox *p;
	u32 i;

	p = (GF_PaddingBitsBox *)a;
	gf_isom_box_dump_start(a, "PaddingBitsBox", trace);
	fprintf(trace, "EntryCount=\"%d\">\n", p->SampleCount);
	for (i=0; i<p->SampleCount; i+=1) {
		fprintf(trace, "<PaddingBitsEntry PaddingBits=\"%d\"/>\n", p->padbits[i]);
	}
	if (!p->size) {
		fprintf(trace, "<PaddingBitsEntry PaddingBits=\"\"/>\n");
	}
	gf_isom_box_dump_done("PaddingBitsBox", a, trace);
	return GF_OK;
}
