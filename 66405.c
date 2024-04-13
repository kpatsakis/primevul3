GF_Err piff_psec_dump(GF_Box *a, FILE * trace)
{
	u32 i, j, sample_count;
	GF_SampleEncryptionBox *ptr = (GF_SampleEncryptionBox *) a;
	if (!a) return GF_BAD_PARAM;

	gf_isom_box_dump_start(a, "PIFFSampleEncryptionBox", trace);
	sample_count = gf_list_count(ptr->samp_aux_info);
	fprintf(trace, "sampleCount=\"%d\"", sample_count);
	if (ptr->flags & 1) {
		fprintf(trace, " AlgorithmID=\"%d\" IV_size=\"%d\" KID=\"", ptr->AlgorithmID, ptr->IV_size);
		dump_data(trace, (char *) ptr->KID, 16);
		fprintf(trace, "\"");
	}
	fprintf(trace, ">\n");

	if (sample_count) {
		for (i=0; i<sample_count; i++) {
			GF_CENCSampleAuxInfo *cenc_sample = (GF_CENCSampleAuxInfo *)gf_list_get(ptr->samp_aux_info, i);

			if (cenc_sample) {
				if  (!strlen((char *)cenc_sample->IV)) continue;
				fprintf(trace, "<PIFFSampleEncryptionEntry IV_size=\"%u\" IV=\"", cenc_sample->IV_size);
				dump_data_hex(trace, (char *) cenc_sample->IV, cenc_sample->IV_size);
				if (ptr->flags & 0x2) {
					fprintf(trace, "\" SubsampleCount=\"%d\"", cenc_sample->subsample_count);
					fprintf(trace, ">\n");

					for (j=0; j<cenc_sample->subsample_count; j++) {
						fprintf(trace, "<PIFFSubSampleEncryptionEntry NumClearBytes=\"%d\" NumEncryptedBytes=\"%d\"/>\n", cenc_sample->subsamples[j].bytes_clear_data, cenc_sample->subsamples[j].bytes_encrypted_data);
					}
				}
				fprintf(trace, "</PIFFSampleEncryptionEntry>\n");
			}
		}
	}
	if (!ptr->size) {
		fprintf(trace, "<PIFFSampleEncryptionEntry IV=\"\" SubsampleCount=\"\">\n");
		fprintf(trace, "<PIFFSubSampleEncryptionEntry NumClearBytes=\"\" NumEncryptedBytes=\"\"/>\n");
		fprintf(trace, "</PIFFSampleEncryptionEntry>\n");
	}
	gf_isom_box_dump_done("PIFFSampleEncryptionBox", a, trace);
	return GF_OK;
}
