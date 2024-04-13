static jas_iccattrval_t *jas_iccattrval_create0()
{
	jas_iccattrval_t *attrval;
	if (!(attrval = jas_malloc(sizeof(jas_iccattrval_t))))
		return 0;
	memset(attrval, 0, sizeof(jas_iccattrval_t));
	attrval->refcnt = 0;
	attrval->ops = 0;
	attrval->type = 0;
	return attrval;
}
