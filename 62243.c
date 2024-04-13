SPL_METHOD(SplObjectStorage, setInfo)
{
	spl_SplObjectStorageElement *element;
	spl_SplObjectStorage *intern = Z_SPLOBJSTORAGE_P(getThis());
	zval *inf;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "z", &inf) == FAILURE) {
		return;
	}

	if ((element = zend_hash_get_current_data_ptr_ex(&intern->storage, &intern->pos)) == NULL) {
		return;
	}
	zval_ptr_dtor(&element->inf);
	ZVAL_COPY(&element->inf, inf);
} /* }}} */

/* {{{ proto void SplObjectStorage::next()
