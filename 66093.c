void stsf_del(GF_Box *s)
{
	u32 nb_entries;
	u32 i;
	GF_StsfEntry *pe;
	GF_SampleFragmentBox *ptr = (GF_SampleFragmentBox *)s;
	if (ptr == NULL) return;

	if (ptr->entryList) {
		nb_entries = gf_list_count(ptr->entryList);
		for ( i = 0; i < nb_entries; i++ ) {
			pe = (GF_StsfEntry*)gf_list_get(ptr->entryList, i);
			if (pe->fragmentSizes) gf_free(pe->fragmentSizes);
			gf_free(pe);
		}
		gf_list_del(ptr->entryList);
	}
	gf_free(ptr);
}
