GF_Err mfra_dump(GF_Box *a, FILE * trace)
{
	GF_MovieFragmentRandomAccessBox *p = (GF_MovieFragmentRandomAccessBox *)a;
	u32 i, count;
	GF_TrackFragmentRandomAccessBox *tfra;

	gf_isom_box_dump_start(a, "MovieFragmentRandomAccessBox", trace);
	fprintf(trace, ">\n");
	count = gf_list_count(p->tfra_list);
	for (i=0; i<count; i++) {
		tfra = (GF_TrackFragmentRandomAccessBox *)gf_list_get(p->tfra_list, i);
		gf_isom_box_dump_ex(tfra, trace, GF_ISOM_BOX_TYPE_TFRA);
	}
	gf_isom_box_dump_done("MovieFragmentRandomAccessBox", a, trace);
	return GF_OK;
}
