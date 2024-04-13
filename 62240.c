SPL_METHOD(SplObjectStorage, key)
{
	spl_SplObjectStorage *intern = Z_SPLOBJSTORAGE_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	RETURN_LONG(intern->index);
} /* }}} */

/* {{{ proto mixed SplObjectStorage::current()
