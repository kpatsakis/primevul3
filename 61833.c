static void jas_icctxt_dump(jas_iccattrval_t *attrval, FILE *out)
{
	jas_icctxt_t *txt = &attrval->data.txt;
	fprintf(out, "string = \"%s\"\n", txt->string);
}
