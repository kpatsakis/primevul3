void gf_propalloc_del(void *it)
{
	GF_PropertyEntry *pe = (GF_PropertyEntry *)it;
	if (pe->prop.value.data.ptr) gf_free(pe->prop.value.data.ptr);
	gf_free(pe);
}