static void jas_iccattrtab_destroy(jas_iccattrtab_t *tab)
{
	if (tab->attrs) {
		while (tab->numattrs > 0) {
			jas_iccattrtab_delete(tab, 0);
		}
		jas_free(tab->attrs);
	}
	jas_free(tab);
}
