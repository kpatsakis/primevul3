GF_Err dref_dump(GF_Box *a, FILE * trace)
{
	gf_isom_box_dump_start(a, "DataReferenceBox", trace);
	fprintf(trace, ">\n");
	gf_isom_box_dump_done("DataReferenceBox", a, trace);
	return GF_OK;
}
