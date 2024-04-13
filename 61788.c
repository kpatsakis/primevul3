static int jas_iccattrtab_lookup(jas_iccattrtab_t *attrtab,
  jas_iccuint32_t name)
{
	int i;
	jas_iccattr_t *attr;
	for (i = 0; i < attrtab->numattrs; ++i) {
		attr = &attrtab->attrs[i];
		if (attr->name == name)
			return i;
	}
	return -1;
}
