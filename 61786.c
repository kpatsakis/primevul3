void jas_iccattrtab_dump(jas_iccattrtab_t *attrtab, FILE *out)
{
	int i;
	jas_iccattr_t *attr;
	jas_iccattrval_t *attrval;
	jas_iccattrvalinfo_t *info;
	char buf[16];
	fprintf(out, "numattrs=%d\n", attrtab->numattrs);
	fprintf(out, "---\n");
	for (i = 0; i < attrtab->numattrs; ++i) {
		attr = &attrtab->attrs[i];
		attrval = attr->val;
		info = jas_iccattrvalinfo_lookup(attrval->type);
		if (!info) abort();
		fprintf(out, "attrno=%d; attrname=\"%s\"(0x%08"PRIxFAST32"); attrtype=\"%s\"(0x%08"PRIxFAST32")\n",
		  i,
		  jas_iccsigtostr(attr->name, &buf[0]),
		  attr->name,
		  jas_iccsigtostr(attrval->type, &buf[8]),
		  attrval->type
		  );
		jas_iccattrval_dump(attrval, out);
		fprintf(out, "---\n");
	}
}
