GF_Err tmax_dump(GF_Box *a, FILE * trace)
{
	GF_TMAXBox *p;
	p = (GF_TMAXBox *)a;
	gf_isom_box_dump_start(a, "MaxTransmissionTimeBox", trace);
	fprintf(trace, "MaximumTransmitTime=\"%d\">\n", p->maxTime);
	gf_isom_box_dump_done("MaxTransmissionTimeBox", a, trace);
	return GF_OK;
}
