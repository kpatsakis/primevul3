GF_Err trgr_dump(GF_Box *a, FILE * trace)
{
	GF_TrackGroupBox *ptr = (GF_TrackGroupBox *) a;
	gf_isom_box_dump_start(a, "TrackGroupBox", trace);
	fprintf(trace, ">\n");
	gf_isom_box_array_dump(ptr->groups, trace);
	gf_isom_box_dump_done("TrackGroupBox", a, trace);
	return GF_OK;
}
