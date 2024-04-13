SPL_METHOD(SplObjectStorage, addAll)
{
	zval *obj;
	spl_SplObjectStorage *intern = Z_SPLOBJSTORAGE_P(getThis());
	spl_SplObjectStorage *other;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "O", &obj, spl_ce_SplObjectStorage) == FAILURE) {
		return;
	}

	other = Z_SPLOBJSTORAGE_P(obj);

	spl_object_storage_addall(intern, getThis(), other);

	RETURN_LONG(zend_hash_num_elements(&intern->storage));
} /* }}} */

/* {{{ proto bool SplObjectStorage::removeAll(SplObjectStorage $os)
