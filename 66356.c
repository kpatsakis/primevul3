GF_Err iSLT_dump(GF_Box *a, FILE * trace)
{
	GF_ISMACrypSaltBox *p = (GF_ISMACrypSaltBox *)a;
	gf_isom_box_dump_start(a, "ISMACrypSaltBox", trace);
	fprintf(trace, "salt=\""LLU"\">\n", p->salt);
	gf_isom_box_dump_done("ISMACrypSaltBox", a, trace);
	return GF_OK;
}
