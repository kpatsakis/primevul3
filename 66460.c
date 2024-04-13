GF_Err tref_dump(GF_Box *a, FILE * trace)
{
	gf_isom_box_dump_start(a, "TrackReferenceBox", trace);
	fprintf(trace, ">\n");
	gf_isom_box_dump_done("TrackReferenceBox", a, trace);
	return GF_OK;
}
