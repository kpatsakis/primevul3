GF_Err piff_tenc_dump(GF_Box *a, FILE * trace)
{
	GF_PIFFTrackEncryptionBox *ptr = (GF_PIFFTrackEncryptionBox*) a;
	if (!a) return GF_BAD_PARAM;

	gf_isom_box_dump_start(a, "PIFFTrackEncryptionBox", trace);

	fprintf(trace, "AlgorithmID=\"%d\" IV_size=\"%d\" KID=\"", ptr->AlgorithmID, ptr->IV_size);
	dump_data_hex(trace,(char *) ptr->KID, 16);
	fprintf(trace, "\">\n");
	gf_isom_box_dump_done("PIFFTrackEncryptionBox", a, trace);
	return GF_OK;
}
