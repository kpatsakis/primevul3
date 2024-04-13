GF_Err rtpo_dump(GF_Box *a, FILE * trace)
{
	GF_RTPOBox *p;
	p = (GF_RTPOBox *)a;
	gf_isom_box_dump_start(a, "RTPTimeOffsetBox", trace);
	fprintf(trace, "PacketTimeOffset=\"%d\">\n", p->timeOffset);
	gf_isom_box_dump_done("RTPTimeOffsetBox", a, trace);
	return GF_OK;
}
