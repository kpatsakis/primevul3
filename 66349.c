GF_Err hinf_dump(GF_Box *a, FILE * trace)
{
	gf_isom_box_dump_start(a, "HintInfoBox", trace);
	fprintf(trace, ">\n");
	gf_isom_box_dump_done("HintInfoBox", a, trace);
	return GF_OK;
}
