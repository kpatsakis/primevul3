GF_Err name_dump(GF_Box *a, FILE * trace)
{
	GF_NameBox *p;
	p = (GF_NameBox *)a;
	gf_isom_box_dump_start(a, "NameBox", trace);
	fprintf(trace, "Name=\"%s\">\n", p->string);
	gf_isom_box_dump_done("NameBox", a, trace);
	return GF_OK;
}
