GF_Err dmed_dump(GF_Box *a, FILE * trace)
{
	GF_DMEDBox *p;

	p = (GF_DMEDBox *)a;
	gf_isom_box_dump_start(a, "BytesFromMediaTrackBox", trace);
	fprintf(trace, "BytesSent=\""LLD"\">\n", LLD_CAST p->nbBytes);
	gf_isom_box_dump_done("BytesFromMediaTrackBox", a, trace);
	return GF_OK;
}
