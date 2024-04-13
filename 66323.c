GF_Err fdsa_dump(GF_Box *a, FILE * trace)
{
	GF_Err e;
	GF_HintSample *ptr = (GF_HintSample *) a;
	if (!a) return GF_BAD_PARAM;

	gf_isom_box_dump_start(a, "FDSampleBox", trace);
	fprintf(trace, ">\n");

	e = gf_isom_box_array_dump(ptr->packetTable, trace);
	if (e) return e;
	if (ptr->extra_data) {
		e = gf_isom_box_dump((GF_Box *)ptr->extra_data, trace);
		if (e) return e;
	}
	gf_isom_box_dump_done("FDSampleBox", a, trace);
	return GF_OK;
}
