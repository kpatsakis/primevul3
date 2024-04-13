GF_Err aeib_dump(GF_Box *a, FILE * trace)
{
	GF_AdobeEncryptionInfoBox *ptr = (GF_AdobeEncryptionInfoBox *)a;
	if (!a) return GF_BAD_PARAM;
	gf_isom_box_dump_start(a, "AdobeEncryptionInfoBox", trace);
	fprintf(trace, "EncryptionAlgorithm=\"%s\" KeyLength=\"%d\">\n", ptr->enc_algo, ptr->key_length);
	gf_isom_box_dump_done("AdobeEncryptionInfoBox", a, trace);
	return GF_OK;
}
