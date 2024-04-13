GF_Err tmin_dump(GF_Box *a, FILE * trace)
{
	GF_TMINBox *p;
	p = (GF_TMINBox *)a;
	gf_isom_box_dump_start(a, "MinTransmissionTimeBox", trace);
	fprintf(trace, "MinimumTransmitTime=\"%d\">\n", p->minTime);
	gf_isom_box_dump_done("MinTransmissionTimeBox", a, trace);
	return GF_OK;
}
