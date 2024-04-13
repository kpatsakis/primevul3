static int jas_icctxt_getsize(jas_iccattrval_t *attrval)
{
	jas_icctxt_t *txt = &attrval->data.txt;
	return JAS_CAST(int, strlen(txt->string) + 1);
}
