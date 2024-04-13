GF_Err frma_dump(GF_Box *a, FILE * trace)
{
	GF_OriginalFormatBox *p;
	p = (GF_OriginalFormatBox *)a;
	gf_isom_box_dump_start(a, "OriginalFormatBox", trace);
	fprintf(trace, "data_format=\"%s\">\n", gf_4cc_to_str(p->data_format));
	gf_isom_box_dump_done("OriginalFormatBox", a, trace);
	return GF_OK;
}
