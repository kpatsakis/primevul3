GF_Err tpyl_dump(GF_Box *a, FILE * trace)
{
	GF_NTYLBox *p;
	p = (GF_NTYLBox *)a;
	gf_isom_box_dump_start(a, "LargeTotalMediaBytesBox", trace);
	fprintf(trace, "BytesSent=\""LLD"\">\n", LLD_CAST p->nbBytes);
	gf_isom_box_dump_done("LargeTotalMediaBytesBox", a, trace);
	return GF_OK;
}
