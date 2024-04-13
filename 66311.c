GF_Err drep_dump(GF_Box *a, FILE * trace)
{
	GF_DREPBox *p;
	p = (GF_DREPBox *)a;
	gf_isom_box_dump_start(a, "RepeatedDataBytesBox", trace);
	fprintf(trace, "RepeatedBytes=\""LLD"\">\n", LLD_CAST p->nbBytes);
	gf_isom_box_dump_done("RepeatedDataBytesBox", a, trace);
	return GF_OK;
}
