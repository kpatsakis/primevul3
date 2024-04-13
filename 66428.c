GF_Err smhd_dump(GF_Box *a, FILE * trace)
{
	gf_isom_box_dump_start(a, "SoundMediaHeaderBox", trace);
	fprintf(trace, ">\n");
	gf_isom_box_dump_done("SoundMediaHeaderBox", a, trace);
	return GF_OK;
}
