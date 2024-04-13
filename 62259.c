static int spl_object_storage_compare_objects(zval *o1, zval *o2) /* {{{ */
{
	zend_object *zo1 = (zend_object *)Z_OBJ_P(o1);
	zend_object *zo2 = (zend_object *)Z_OBJ_P(o2);

	if (zo1->ce != spl_ce_SplObjectStorage || zo2->ce != spl_ce_SplObjectStorage) {
		return 1;
	}

	return zend_hash_compare(&(Z_SPLOBJSTORAGE_P(o1))->storage, &(Z_SPLOBJSTORAGE_P(o2))->storage, (compare_func_t)spl_object_storage_compare_info, 0);
}
/* }}} */
