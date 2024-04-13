GF_Err totl_dump(GF_Box *a, FILE * trace)
{
	GF_TOTLBox *p;

	p = (GF_TOTLBox *)a;
	gf_isom_box_dump_start(a, "TotalRTPBytesBox", trace);
	fprintf(trace, "RTPBytesSent=\"%d\">\n", p->nbBytes);
	gf_isom_box_dump_done("TotalRTPBytesBox", a, trace);
	return GF_OK;
}
