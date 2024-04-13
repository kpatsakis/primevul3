static void jas_iccattrtab_delete(jas_iccattrtab_t *attrtab, int i)
{
	int n;
	jas_iccattrval_destroy(attrtab->attrs[i].val);
	if ((n = attrtab->numattrs - i - 1) > 0)
		memmove(&attrtab->attrs[i], &attrtab->attrs[i + 1],
		  n * sizeof(jas_iccattr_t));
	--attrtab->numattrs;
}
