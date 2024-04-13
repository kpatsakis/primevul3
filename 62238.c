SPL_METHOD(SplObjectStorage, contains)
{
	zval *obj;
	spl_SplObjectStorage *intern = Z_SPLOBJSTORAGE_P(getThis());

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "o", &obj) == FAILURE) {
		return;
	}
	RETURN_BOOL(spl_object_storage_contains(intern, getThis(), obj));
} /* }}} */

/* {{{ proto int SplObjectStorage::count()
