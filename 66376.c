GF_Err maxr_dump(GF_Box *a, FILE * trace)
{
	GF_MAXRBox *p;
	p = (GF_MAXRBox *)a;
	gf_isom_box_dump_start(a, "MaxDataRateBox", trace);
	fprintf(trace, "MaxDataRate=\"%d\" Granularity=\"%d\">\n", p->maxDataRate, p->granularity);
	gf_isom_box_dump_done("MaxDataRateBox", a, trace);
	return GF_OK;
}
