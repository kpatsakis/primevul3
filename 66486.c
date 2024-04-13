GF_Err uuid_dump(GF_Box *a, FILE * trace)
{
	gf_isom_box_dump_start(a, "UUIDBox", trace);
	fprintf(trace, ">\n");
	gf_isom_box_dump_done("UUIDBox", a, trace);
	return GF_OK;
}
