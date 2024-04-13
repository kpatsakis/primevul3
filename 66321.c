GF_Err extr_dump(GF_Box *a, FILE * trace)
{
	GF_ExtraDataBox *ptr = (GF_ExtraDataBox *) a;
	if (!a) return GF_BAD_PARAM;
	gf_isom_box_dump_start(a, "ExtraDataBox", trace);
	dump_data_attribute(trace, "data", ptr->data, ptr->data_length);
	fprintf(trace, ">\n");
	if (ptr->feci) {
		gf_isom_box_dump((GF_Box *)ptr->feci, trace);
	}
	gf_isom_box_dump_done("ExtraDataBox", a, trace);
	return GF_OK;
}
