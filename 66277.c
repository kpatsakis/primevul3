GF_Err adkm_dump(GF_Box *a, FILE * trace)
{
	GF_AdobeDRMKeyManagementSystemBox *ptr = (GF_AdobeDRMKeyManagementSystemBox *)a;
	if (!a) return GF_BAD_PARAM;
	gf_isom_box_dump_start(a, "AdobeDRMKeyManagementSystemBox", trace);

	fprintf(trace, ">\n");
	if (ptr->header) gf_isom_box_dump((GF_Box *)ptr->header, trace);
	if (ptr->au_format) gf_isom_box_dump((GF_Box *)ptr->au_format, trace);
	gf_isom_box_dump_done("AdobeDRMKeyManagementSystemBox", a, trace);
	return GF_OK;
}
