static int jas_iccattrtab_replace(jas_iccattrtab_t *attrtab, int i,
  jas_iccuint32_t name, jas_iccattrval_t *val)
{
	jas_iccattrval_t *newval;
	jas_iccattr_t *attr;
	if (!(newval = jas_iccattrval_clone(val)))
		goto error;
	attr = &attrtab->attrs[i];
	jas_iccattrval_destroy(attr->val);
	attr->name = name;
	attr->val = newval;
	return 0;
error:
	return -1;
}
