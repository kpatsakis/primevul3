GF_Err rloc_dump(GF_Box *a, FILE * trace)
{
	GF_RelativeLocationPropertyBox *ptr = (GF_RelativeLocationPropertyBox *)a;
	if (!a) return GF_BAD_PARAM;
	gf_isom_box_dump_start(a, "RelativeLocationPropertyBox", trace);
	fprintf(trace, "horizontal_offset=\"%d\" vertical_offset=\"%d\">\n", ptr->horizontal_offset, ptr->vertical_offset);
	gf_isom_box_dump_done("RelativeLocationPropertyBox", a, trace);
	return GF_OK;
}
