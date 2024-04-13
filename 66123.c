GF_Err subs_Size(GF_Box *s)
{
	GF_SubSampleInformationBox *ptr = (GF_SubSampleInformationBox *) s;
	GF_SubSampleInfoEntry *pSamp;
	u32 entry_count, i;
	u16 subsample_count;

	ptr->size += 4;
	entry_count = gf_list_count(ptr->Samples);
	for (i=0; i<entry_count; i++) {
		pSamp = (GF_SubSampleInfoEntry*) gf_list_get(ptr->Samples, i);
		subsample_count = gf_list_count(pSamp->SubSamples);
		ptr->size += 4 + 2 + subsample_count * (6 + (ptr->version==1 ? 4 : 2));
	}
	return GF_OK;
}
