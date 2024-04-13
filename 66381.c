GF_Err mehd_dump(GF_Box *a, FILE * trace)
{
	GF_MovieExtendsHeaderBox *p = (GF_MovieExtendsHeaderBox*)a;
	gf_isom_box_dump_start(a, "MovieExtendsHeaderBox", trace);
	fprintf(trace, "fragmentDuration=\""LLD"\" >\n", LLD_CAST p->fragment_duration);
	gf_isom_box_dump_done("MovieExtendsHeaderBox", a, trace);
	return GF_OK;
}
