static int jas_icclut16_getsize(jas_iccattrval_t *attrval)
{
	jas_icclut16_t *lut16 = &attrval->data.lut16;
	return 44 + 2 * (lut16->numinchans * lut16->numintabents +
	  lut16->numoutchans * lut16->numouttabents +
	  jas_iccpowi(lut16->clutlen, lut16->numinchans) * lut16->numoutchans);
}
