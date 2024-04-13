GF_Err subs_dump(GF_Box *a, FILE * trace)
{
	u32 entry_count, i, j;
	u16 subsample_count;
	GF_SubSampleInfoEntry *pSamp;
	GF_SubSampleEntry *pSubSamp;
	GF_SubSampleInformationBox *ptr = (GF_SubSampleInformationBox *) a;

	if (!a) return GF_BAD_PARAM;

	entry_count = gf_list_count(ptr->Samples);
	gf_isom_box_dump_start(a, "SubSampleInformationBox", trace);

	fprintf(trace, "EntryCount=\"%d\">\n", entry_count);

	for (i=0; i<entry_count; i++) {
		pSamp = (GF_SubSampleInfoEntry*) gf_list_get(ptr->Samples, i);

		subsample_count = gf_list_count(pSamp->SubSamples);

		fprintf(trace, "<SampleEntry SampleDelta=\"%d\" SubSampleCount=\"%d\">\n", pSamp->sample_delta, subsample_count);

		for (j=0; j<subsample_count; j++) {
			pSubSamp = (GF_SubSampleEntry*) gf_list_get(pSamp->SubSamples, j);
			fprintf(trace, "<SubSample Size=\"%u\" Priority=\"%u\" Discardable=\"%d\" Reserved=\"%08X\"/>\n", pSubSamp->subsample_size, pSubSamp->subsample_priority, pSubSamp->discardable, pSubSamp->reserved);
		}
		fprintf(trace, "</SampleEntry>\n");
	}
	if (!ptr->size) {
		fprintf(trace, "<SampleEntry SampleDelta=\"\" SubSampleCount=\"\">\n");
		fprintf(trace, "<SubSample Size=\"\" Priority=\"\" Discardable=\"\" Reserved=\"\"/>\n");
		fprintf(trace, "</SampleEntry>\n");
	}

	gf_isom_box_dump_done("SubSampleInformationBox", a, trace);
	return GF_OK;
}
