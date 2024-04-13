int jas_iccattrval_allowmodify(jas_iccattrval_t **attrvalx)
{
	jas_iccattrval_t *newattrval;
	jas_iccattrval_t *attrval = *attrvalx;
	newattrval = 0;
	if (attrval->refcnt > 1) {
		if (!(newattrval = jas_iccattrval_create0()))
			goto error;
		newattrval->ops = attrval->ops;
		newattrval->type = attrval->type;
		++newattrval->refcnt;
		if (newattrval->ops->copy) {
			if ((*newattrval->ops->copy)(newattrval, attrval))
				goto error;
		} else {
			memcpy(&newattrval->data, &attrval->data,
			  sizeof(newattrval->data));
		}
		*attrvalx = newattrval;
	}
	return 0;
error:
	if (newattrval) {
		jas_free(newattrval);
	}
	return -1;
}
