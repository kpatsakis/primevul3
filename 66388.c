GF_Err moof_dump(GF_Box *a, FILE * trace)
{
	GF_MovieFragmentBox *p;
	p = (GF_MovieFragmentBox *)a;
	gf_isom_box_dump_start(a, "MovieFragmentBox", trace);
	fprintf(trace, "TrackFragments=\"%d\">\n", gf_list_count(p->TrackList));
	if (p->mfhd) gf_isom_box_dump(p->mfhd, trace);
	gf_isom_box_array_dump(p->TrackList, trace);
	gf_isom_box_dump_done("MovieFragmentBox", a, trace);
	return GF_OK;
}
