GF_Err tpay_dump(GF_Box *a, FILE * trace)
{
	GF_TPAYBox *p;
	p = (GF_TPAYBox *)a;
	gf_isom_box_dump_start(a, "TotalMediaBytesBox", trace);
	fprintf(trace, "BytesSent=\"%d\">\n", p->nbBytes);
	gf_isom_box_dump_done("TotalMediaBytesBox", a, trace);
	return GF_OK;
}
