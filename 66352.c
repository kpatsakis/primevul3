GF_Err hnti_dump(GF_Box *a, FILE * trace)
{
	gf_isom_box_dump_start(a, "HintTrackInfoBox", trace);
	fprintf(trace, ">\n");
	gf_isom_box_dump_done("HintTrackInfoBox", NULL, trace);
	return GF_OK;
}
