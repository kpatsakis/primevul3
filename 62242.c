SPL_METHOD(SplObjectStorage, getInfo)
{
	spl_SplObjectStorageElement *element;
	spl_SplObjectStorage *intern = Z_SPLOBJSTORAGE_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	if ((element = zend_hash_get_current_data_ptr_ex(&intern->storage, &intern->pos)) == NULL) {
		return;
	}
	ZVAL_COPY(return_value, &element->inf);
} /* }}} */

/* {{{ proto mixed SplObjectStorage::setInfo(mixed $inf)
