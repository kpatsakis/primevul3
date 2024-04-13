GF_Err stsf_Read(GF_Box *s, GF_BitStream *bs)
{
	u32 entries, i;
	u32 nb_entries;
	GF_StsfEntry *p;
	GF_SampleFragmentBox *ptr = (GF_SampleFragmentBox *)s;

	p = NULL;
	if (!ptr) return GF_BAD_PARAM;
	nb_entries = gf_bs_read_u32(bs);

	p = NULL;
	for ( entries = 0; entries < nb_entries; entries++ ) {
		p = (GF_StsfEntry *) gf_malloc(sizeof(GF_StsfEntry));
		if (!p) return GF_OUT_OF_MEM;
		p->SampleNumber = gf_bs_read_u32(bs);
		p->fragmentCount = gf_bs_read_u32(bs);
		p->fragmentSizes = (u16*)gf_malloc(sizeof(GF_StsfEntry) * p->fragmentCount);
		for (i=0; i<p->fragmentCount; i++) {
			p->fragmentSizes[i] = gf_bs_read_u16(bs);
		}
		gf_list_add(ptr->entryList, p);
	}
#ifndef GPAC_DISABLE_ISOM_WRITE
	ptr->w_currentEntry = p;
	ptr->w_currentEntryIndex = nb_entries-1;
#endif
	return GF_OK;
}
