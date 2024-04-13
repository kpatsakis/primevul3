GF_Err vmhd_dump(GF_Box *a, FILE * trace)
{
	gf_isom_box_dump_start(a, "VideoMediaHeaderBox", trace);
	fprintf(trace, ">\n");
	gf_isom_box_dump_done("VideoMediaHeaderBox", a, trace);
	return GF_OK;
}
