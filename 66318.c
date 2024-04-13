GF_Err elng_dump(GF_Box *a, FILE * trace)
{
	GF_ExtendedLanguageBox *p = (GF_ExtendedLanguageBox *)a;
	gf_isom_box_dump_start(a, "ExtendedLanguageBox", trace);
	fprintf(trace, "LanguageCode=\"%s\">\n", p->extended_language);
	gf_isom_box_dump_done("ExtendedLanguageBox", a, trace);
	return GF_OK;
}
