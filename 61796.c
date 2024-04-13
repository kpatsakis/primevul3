void jas_iccattrval_dump(jas_iccattrval_t *attrval, FILE *out)
{
	char buf[8];
	jas_iccsigtostr(attrval->type, buf);
	fprintf(out, "refcnt = %d; type = 0x%08"PRIxFAST32" %s\n", attrval->refcnt,
	  attrval->type, jas_iccsigtostr(attrval->type, &buf[0]));
	if (attrval->ops->dump) {
		(*attrval->ops->dump)(attrval, out);
	}
}
