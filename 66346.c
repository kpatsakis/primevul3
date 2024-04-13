GF_Err grpl_dump(GF_Box *a, FILE * trace)
{
	gf_isom_box_dump_start(a, "GroupListBox", trace);
	fprintf(trace, ">\n");
	gf_isom_box_dump_done("GroupListBox", a, trace);
	return GF_OK;
}
