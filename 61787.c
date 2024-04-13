static int jas_iccattrtab_get(jas_iccattrtab_t *attrtab, int i,
  jas_iccattrname_t *name, jas_iccattrval_t **val)
{
	jas_iccattr_t *attr;
	if (i < 0 || i >= attrtab->numattrs)
		goto error;
	attr = &attrtab->attrs[i];
	*name = attr->name;
	if (!(*val = jas_iccattrval_clone(attr->val)))
		goto error;
	return 0;
error:
	return -1;
}
