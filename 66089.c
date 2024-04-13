GF_Box *stsf_New()
{
	ISOM_DECL_BOX_ALLOC(GF_SampleFragmentBox, GF_ISOM_BOX_TYPE_STSF);

	tmp->entryList = gf_list_new();
	if (! tmp->entryList) {
		gf_free(tmp);
		return NULL;
	}
	return (GF_Box *) tmp;
}
