GF_Err def_cont_box_dump(GF_Box *a, FILE *trace)
{
	char *name = "SubTrackDefinitionBox"; //only one using generic box container for now
	gf_isom_box_dump_start(a, name, trace);
	fprintf(trace, ">\n");
	gf_isom_box_dump_done(name, a, trace);
	return GF_OK;
}
