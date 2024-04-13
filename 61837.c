static int jas_icctxtdesc_copy(jas_iccattrval_t *attrval,
  jas_iccattrval_t *othattrval)
{
	jas_icctxtdesc_t *txtdesc = &attrval->data.txtdesc;

	/* Avoid compiler warnings about unused parameters. */
	attrval = 0;
	othattrval = 0;
	txtdesc = 0;

	/* Not yet implemented. */
	abort();
	return -1;
}
