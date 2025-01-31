SPL_METHOD(SplObjectStorage, attach)
{
	zval *obj, *inf = NULL;

	spl_SplObjectStorage *intern = Z_SPLOBJSTORAGE_P(getThis());

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "o|z!", &obj, &inf) == FAILURE) {
		return;
	}
	spl_object_storage_attach(intern, getThis(), obj, inf);
} /* }}} */

/* {{{ proto void SplObjectStorage::detach(object obj)
