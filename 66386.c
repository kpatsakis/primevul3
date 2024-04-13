GF_Err mfro_dump(GF_Box *a, FILE * trace)
{
	GF_MovieFragmentRandomAccessOffsetBox *p = (GF_MovieFragmentRandomAccessOffsetBox *)a;

	gf_isom_box_dump_start(a, "MovieFragmentRandomAccessOffsetBox", trace);

	fprintf(trace, "container_size=\"%d\" >\n", p->container_size);
	gf_isom_box_dump_done("MovieFragmentRandomAccessOffsetBox", a, trace);
	return GF_OK;
}
