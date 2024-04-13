static int jas_icclut8_getsize(jas_iccattrval_t *attrval)
{
	jas_icclut8_t *lut8 = &attrval->data.lut8;
	return 44 + lut8->numinchans * lut8->numintabents +
	  lut8->numoutchans * lut8->numouttabents +
	  jas_iccpowi(lut8->clutlen, lut8->numinchans) * lut8->numoutchans;
}
