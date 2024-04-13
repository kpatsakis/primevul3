GF_Err stsd_dump(GF_Box *a, FILE * trace)
{
	gf_isom_box_dump_start(a, "SampleDescriptionBox", trace);
	fprintf(trace, ">\n");
	gf_isom_box_dump_done("SampleDescriptionBox", a, trace);
	return GF_OK;
}
