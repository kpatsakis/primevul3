GF_Err nmhd_dump(GF_Box *a, FILE * trace)
{
	gf_isom_box_dump_start(a, "MPEGMediaHeaderBox", trace);
	fprintf(trace, ">\n");
	gf_isom_box_dump_done("MPEGMediaHeaderBox", a, trace);
	return GF_OK;
}
