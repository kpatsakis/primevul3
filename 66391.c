GF_Err mvex_dump(GF_Box *a, FILE * trace)
{
	GF_MovieExtendsBox *p;
	p = (GF_MovieExtendsBox *)a;
	gf_isom_box_dump_start(a, "MovieExtendsBox", trace);
	fprintf(trace, ">\n");
	if (p->mehd) gf_isom_box_dump(p->mehd, trace);
	gf_isom_box_array_dump(p->TrackExList, trace);
	gf_isom_box_array_dump(p->TrackExPropList, trace);
	gf_isom_box_dump_done("MovieExtendsBox", a, trace);
	return GF_OK;
}
