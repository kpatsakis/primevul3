GF_Err ahdr_dump(GF_Box *a, FILE * trace)
{
	GF_AdobeDRMHeaderBox *ptr = (GF_AdobeDRMHeaderBox *)a;
	if (!a) return GF_BAD_PARAM;
	gf_isom_box_dump_start(a, "AdobeDRMHeaderBox", trace);
	fprintf(trace, ">\n");
	if (ptr->std_enc_params) gf_isom_box_dump((GF_Box *)ptr->std_enc_params, trace);
	gf_isom_box_dump_done("AdobeDRMHeaderBox", a, trace);
	return GF_OK;
}
