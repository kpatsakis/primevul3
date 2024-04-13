GF_Err databox_dump(GF_Box *a, FILE * trace)
{
	gf_isom_box_dump_start(a, "data", trace);

	fprintf(trace, ">\n");
	gf_isom_box_dump_done("data", a, trace);
	return GF_OK;
}
