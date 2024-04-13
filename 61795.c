void jas_iccattrval_destroy(jas_iccattrval_t *attrval)
{
#if 0
jas_eprintf("refcnt=%d\n", attrval->refcnt);
#endif
	if (--attrval->refcnt <= 0) {
		if (attrval->ops->destroy)
			(*attrval->ops->destroy)(attrval);
		jas_free(attrval);
	}
}
