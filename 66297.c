GF_Err cprt_dump(GF_Box *a, FILE * trace)
{
	GF_CopyrightBox *p;

	p = (GF_CopyrightBox *)a;
	gf_isom_box_dump_start(a, "CopyrightBox", trace);
	fprintf(trace, "LanguageCode=\"%s\" CopyrightNotice=\"%s\">\n", p->packedLanguageCode, p->notice);
	gf_isom_box_dump_done("CopyrightBox", a, trace);
	return GF_OK;
}
