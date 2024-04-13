find_pa_data(const METHOD_DATA *md, unsigned type)
{
    size_t i;
    if (md == NULL)
	return NULL;
    for (i = 0; i < md->len; i++)
	if (md->val[i].padata_type == type)
	    return &md->val[i];
    return NULL;
}
