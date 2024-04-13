SPL_METHOD(SplObjectStorage, getHash)
{
	zval *obj;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "o", &obj) == FAILURE) {
		return;
	}

	RETURN_NEW_STR(php_spl_object_hash(obj));

} /* }}} */

/* {{{ proto mixed SplObjectStorage::offsetGet(object obj)
