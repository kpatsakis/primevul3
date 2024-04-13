static jas_iccattrtab_t *jas_iccattrtab_create()
{
	jas_iccattrtab_t *tab;
	tab = 0;
	if (!(tab = jas_malloc(sizeof(jas_iccattrtab_t))))
		goto error;
	tab->maxattrs = 0;
	tab->numattrs = 0;
	tab->attrs = 0;
	if (jas_iccattrtab_resize(tab, 32))
		goto error;
	return tab;
error:
	if (tab)
		jas_iccattrtab_destroy(tab);
	return 0;
}
