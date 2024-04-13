static void jas_icctxtdesc_dump(jas_iccattrval_t *attrval, FILE *out)
{
	jas_icctxtdesc_t *txtdesc = &attrval->data.txtdesc;
	fprintf(out, "ascii = \"%s\"\n", txtdesc->ascdata);
	fprintf(out, "uclangcode = %"PRIuFAST32"; uclen = %"PRIuFAST32"\n",
	  txtdesc->uclangcode, txtdesc->uclen);
	fprintf(out, "sccode = %"PRIuFAST16"\n", txtdesc->sccode);
	fprintf(out, "maclen = %d\n", txtdesc->maclen);
}
