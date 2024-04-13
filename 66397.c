GF_Err odrb_dump(GF_Box *a, FILE * trace)
{
	GF_OMADRMRightsObjectBox*ptr = (GF_OMADRMRightsObjectBox*)a;
	gf_isom_box_dump_start(a, "OMADRMRightsObjectBox", trace);

	fprintf(trace, "OMARightsObject=\"");
	dump_data(trace, ptr->oma_ro, ptr->oma_ro_size);
	fprintf(trace, "\">\n");
	gf_isom_box_dump_done("OMADRMRightsObjectBox", a, trace);
	return GF_OK;
}
