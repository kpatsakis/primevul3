SPL_METHOD(SplObjectStorage, next)
{
	spl_SplObjectStorage *intern = Z_SPLOBJSTORAGE_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	zend_hash_move_forward_ex(&intern->storage, &intern->pos);
	intern->index++;
} /* }}} */

/* {{{ proto string SplObjectStorage::serialize()
