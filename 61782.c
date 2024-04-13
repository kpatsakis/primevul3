static int jas_iccattrtab_add(jas_iccattrtab_t *attrtab, int i,
  jas_iccuint32_t name, jas_iccattrval_t *val)
{
	int n;
	jas_iccattr_t *attr;
	jas_iccattrval_t *tmpattrval;
	tmpattrval = 0;
	if (i < 0) {
		i = attrtab->numattrs;
	}
	assert(i >= 0 && i <= attrtab->numattrs);
	if (attrtab->numattrs >= attrtab->maxattrs) {
		if (jas_iccattrtab_resize(attrtab, attrtab->numattrs + 32)) {
			goto error;
		}
	}
	if (!(tmpattrval = jas_iccattrval_clone(val)))
		goto error;
	n = attrtab->numattrs - i;
	if (n > 0)
		memmove(&attrtab->attrs[i + 1], &attrtab->attrs[i],
		  n * sizeof(jas_iccattr_t));
	attr = &attrtab->attrs[i];
	attr->name = name;
	attr->val = tmpattrval;
	++attrtab->numattrs;
	return 0;
error:
	if (tmpattrval)
		jas_iccattrval_destroy(tmpattrval);
	return -1;
}
