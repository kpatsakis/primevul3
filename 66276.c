GF_Err adaf_dump(GF_Box *a, FILE * trace)
{
	GF_AdobeDRMAUFormatBox *ptr = (GF_AdobeDRMAUFormatBox *)a;
	if (!a) return GF_BAD_PARAM;
	gf_isom_box_dump_start(a, "AdobeDRMAUFormatBox ", trace);
	fprintf(trace, "SelectiveEncryption=\"%d\" IV_length=\"%d\">\n", ptr->selective_enc ? 1 : 0, ptr->IV_length);
	gf_isom_box_dump_done("AdobeDRMAUFormatBox", a, trace);
	return GF_OK;
}
