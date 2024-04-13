GF_Err gf_isom_dump_ismacryp_sample(GF_ISOFile *the_file, u32 trackNumber, u32 SampleNum, FILE * trace)
{
	GF_ISOSample *samp;
	GF_ISMASample  *isma_samp;
	u32 descIndex;

	samp = gf_isom_get_sample(the_file, trackNumber, SampleNum, &descIndex);
	if (!samp) return GF_BAD_PARAM;

	isma_samp = gf_isom_get_ismacryp_sample(the_file, trackNumber, samp, descIndex);
	if (!isma_samp) {
		gf_isom_sample_del(&samp);
		return GF_NOT_SUPPORTED;
	}

	fprintf(trace, "<ISMACrypSample SampleNumber=\"%d\" DataSize=\"%d\" CompositionTime=\""LLD"\" ", SampleNum, isma_samp->dataLength, LLD_CAST (samp->DTS+samp->CTS_Offset) );
	if (samp->CTS_Offset) fprintf(trace, "DecodingTime=\""LLD"\" ", LLD_CAST samp->DTS);
	if (gf_isom_has_sync_points(the_file, trackNumber)) fprintf(trace, "RandomAccessPoint=\"%s\" ", samp->IsRAP ? "Yes" : "No");
	fprintf(trace, "IsEncrypted=\"%s\" ", (isma_samp->flags & GF_ISOM_ISMA_IS_ENCRYPTED) ? "Yes" : "No");
	if (isma_samp->flags & GF_ISOM_ISMA_IS_ENCRYPTED) {
		fprintf(trace, "IV=\""LLD"\" ", LLD_CAST isma_samp->IV);
		if (isma_samp->key_indicator) dump_data_attribute(trace, "KeyIndicator", (char*)isma_samp->key_indicator, isma_samp->KI_length);
	}
	fprintf(trace, "/>\n");

	gf_isom_sample_del(&samp);
	gf_isom_ismacryp_delete_sample(isma_samp);
	return GF_OK;
}
