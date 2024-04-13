GF_Err urn_dump(GF_Box *a, FILE * trace)
{
	GF_DataEntryURNBox *p;

	p = (GF_DataEntryURNBox *)a;
	gf_isom_box_dump_start(a, "URNDataEntryBox", trace);
	if (p->nameURN) fprintf(trace, " URN=\"%s\"", p->nameURN);
	if (p->location) fprintf(trace, " URL=\"%s\"", p->location);
	fprintf(trace, ">\n");

	gf_isom_box_dump_done("URNDataEntryBox", a, trace);
	return GF_OK;
}
