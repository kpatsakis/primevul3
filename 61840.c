static int jas_icctxtdesc_getsize(jas_iccattrval_t *attrval)
{
	jas_icctxtdesc_t *txtdesc = &attrval->data.txtdesc;
	return JAS_CAST(int, strlen(txtdesc->ascdata) + 1 + txtdesc->uclen * 2 +
	  15 + 67);
}
