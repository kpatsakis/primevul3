GF_Err iKMS_dump(GF_Box *a, FILE * trace)
{
	GF_ISMAKMSBox *p;
	p = (GF_ISMAKMSBox *)a;
	gf_isom_box_dump_start(a, "KMSBox", trace);
	fprintf(trace, "kms_URI=\"%s\">\n", p->URI);
	gf_isom_box_dump_done("KMSBox", a, trace);
	return GF_OK;

}
