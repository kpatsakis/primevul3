jas_iccattrval_t *jas_iccattrval_clone(jas_iccattrval_t *attrval)
{
	++attrval->refcnt;
	return attrval;
}
