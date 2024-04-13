GF_Err unkn_dump(GF_Box *a, FILE * trace)
{
	GF_UnknownBox *u = (GF_UnknownBox *)a;
	u->type = u->original_4cc;
	gf_isom_box_dump_start(a, "UnknownBox", trace);
	u->type = GF_ISOM_BOX_TYPE_UNKNOWN;
	if (u->dataSize<100)
		dump_data_attribute(trace, "data", u->data, u->dataSize);
	fprintf(trace, ">\n");
	gf_isom_box_dump_done("UnknownBox", a, trace);
	return GF_OK;
}
