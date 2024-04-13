jas_iccattrval_t *jas_iccattrval_create(jas_iccuint32_t type)
{
	jas_iccattrval_t *attrval;
	jas_iccattrvalinfo_t *info;

	if (!(info = jas_iccattrvalinfo_lookup(type)))
		goto error;
	if (!(attrval = jas_iccattrval_create0()))
		goto error;
	attrval->ops = &info->ops;
	attrval->type = type;
	++attrval->refcnt;
	memset(&attrval->data, 0, sizeof(attrval->data));
	return attrval;
error:
	return 0;
}
