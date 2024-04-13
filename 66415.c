GF_Err rssr_dump(GF_Box *a, FILE * trace)
{
	GF_ReceivedSsrcBox *p = (GF_ReceivedSsrcBox *)a;
	gf_isom_box_dump_start(a, "ReceivedSsrcBox", trace);
	fprintf(trace, "SSRC=\"%d\">\n", p->ssrc);
	gf_isom_box_dump_done("ReceivedSsrcBox", a, trace);
	return GF_OK;
}
