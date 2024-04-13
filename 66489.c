GF_Err void_dump(GF_Box *a, FILE * trace)
{
	gf_isom_box_dump_start(a, "VoidBox", trace);
	fprintf(trace, ">\n");
	gf_isom_box_dump_done("VoidBox", a, trace);
	return GF_OK;
}
