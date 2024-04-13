GF_Err stsf_Write(GF_Box *s, GF_BitStream *bs)
{
	GF_Err e;
	u32 i, j;
	u32 nb_entries;
	GF_StsfEntry *p;
	GF_SampleFragmentBox *ptr = (GF_SampleFragmentBox *)s;

	e = gf_isom_full_box_write(s, bs);
	if (e) return e;
	nb_entries = gf_list_count(ptr->entryList);
	gf_bs_write_u32(bs, nb_entries);
	for ( i = 0; i < nb_entries; i++ ) {
		p = (GF_StsfEntry*)gf_list_get(ptr->entryList, i);
		gf_bs_write_u32(bs, p->SampleNumber);
		gf_bs_write_u32(bs, p->fragmentCount);
		for (j=0; j<p->fragmentCount; j++) {
			gf_bs_write_u16(bs, p->fragmentSizes[j]);
		}
	}
	return GF_OK;
}
