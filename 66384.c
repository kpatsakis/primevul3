GF_Err mfhd_dump(GF_Box *a, FILE * trace)
{
	GF_MovieFragmentHeaderBox *p;
	p = (GF_MovieFragmentHeaderBox *)a;
	gf_isom_box_dump_start(a, "MovieFragmentHeaderBox", trace);
	fprintf(trace, "FragmentSequenceNumber=\"%d\">\n", p->sequence_number);
	gf_isom_box_dump_done("MovieFragmentHeaderBox", a, trace);
	return GF_OK;
}
