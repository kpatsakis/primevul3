GF_Err senc_dump(GF_Box *a, FILE * trace)
{
	u32 i, j, sample_count;
	GF_SampleEncryptionBox *ptr = (GF_SampleEncryptionBox *) a;
	if (!a) return GF_BAD_PARAM;

	gf_isom_box_dump_start(a, "SampleEncryptionBox", trace);
	sample_count = gf_list_count(ptr->samp_aux_info);
	fprintf(trace, "sampleCount=\"%d\">\n", sample_count);
	fprintf(trace, "<FullBoxInfo Version=\"%d\" Flags=\"0x%X\"/>\n", ptr->version, ptr->flags);
	for (i=0; i<sample_count; i++) {
		GF_CENCSampleAuxInfo *cenc_sample = (GF_CENCSampleAuxInfo *)gf_list_get(ptr->samp_aux_info, i);

		if (cenc_sample) {
			fprintf(trace, "<SampleEncryptionEntry sampleNumber=\"%d\" IV_size=\"%u\" IV=\"", i+1, cenc_sample->IV_size);
			dump_data_hex(trace, (char *) cenc_sample->IV, cenc_sample->IV_size);
			fprintf(trace, "\"");
			if (ptr->flags & 0x2) {
				fprintf(trace, " SubsampleCount=\"%d\"", cenc_sample->subsample_count);
				fprintf(trace, ">\n");

				for (j=0; j<cenc_sample->subsample_count; j++) {
					fprintf(trace, "<SubSampleEncryptionEntry NumClearBytes=\"%d\" NumEncryptedBytes=\"%d\"/>\n", cenc_sample->subsamples[j].bytes_clear_data, cenc_sample->subsamples[j].bytes_encrypted_data);
				}
			} else {
				fprintf(trace, ">\n");
			}
			fprintf(trace, "</SampleEncryptionEntry>\n");
		}
	}
	if (!ptr->size) {
		fprintf(trace, "<SampleEncryptionEntry sampleCount=\"\" IV=\"\" SubsampleCount=\"\">\n");
		fprintf(trace, "<SubSampleEncryptionEntry NumClearBytes=\"\" NumEncryptedBytes=\"\"/>\n");
		fprintf(trace, "</SampleEncryptionEntry>\n");
	}
	gf_isom_box_dump_done("SampleEncryptionBox", a, trace);
	return GF_OK;
}
