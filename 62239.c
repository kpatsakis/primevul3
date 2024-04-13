SPL_METHOD(SplObjectStorage, count)
{
	spl_SplObjectStorage *intern = Z_SPLOBJSTORAGE_P(getThis());
	zend_long mode = COUNT_NORMAL;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "|l", &mode) == FAILURE) {
		return;
	}

	if (mode == COUNT_RECURSIVE) {
		zend_long ret = zend_hash_num_elements(&intern->storage);
		zval *element;

		ZEND_HASH_FOREACH_VAL(&intern->storage, element) {
			ret += php_count_recursive(element, mode);
		} ZEND_HASH_FOREACH_END();

		RETURN_LONG(ret);
		return;
	}

	RETURN_LONG(zend_hash_num_elements(&intern->storage));
} /* }}} */

/* {{{ proto void SplObjectStorage::rewind()
