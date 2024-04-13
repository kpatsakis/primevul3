GF_Err dimm_dump(GF_Box *a, FILE * trace)
{
	GF_DIMMBox *p;
	p = (GF_DIMMBox *)a;
	gf_isom_box_dump_start(a, "ImmediateDataBytesBox", trace);
	fprintf(trace, "BytesSent=\""LLD"\">\n", LLD_CAST p->nbBytes);
	gf_isom_box_dump_done("ImmediateDataBytesBox", a, trace);
	return GF_OK;
}
