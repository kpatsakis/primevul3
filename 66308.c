GF_Err dmax_dump(GF_Box *a, FILE * trace)
{
	GF_DMAXBox *p;
	p = (GF_DMAXBox *)a;
	gf_isom_box_dump_start(a, "MaxPacketDurationBox", trace);
	fprintf(trace, "MaximumDuration=\"%d\">\n", p->maxDur);
	gf_isom_box_dump_done("MaxPacketDurationBox", a, trace);
	return GF_OK;
}
