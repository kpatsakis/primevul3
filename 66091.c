GF_Err stsf_Size(GF_Box *s)
{
	GF_StsfEntry *p;
	u32 nb_entries, i;
	GF_SampleFragmentBox *ptr = (GF_SampleFragmentBox *) s;

	nb_entries = gf_list_count(ptr->entryList);
	ptr->size += 4;
	for (i=0; i<nb_entries; i++) {
		p = (GF_StsfEntry *)gf_list_get(ptr->entryList, i);
		ptr->size += 8 + 2*p->fragmentCount;
	}
	return GF_OK;
}
