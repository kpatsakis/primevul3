SPL_METHOD(MultipleIterator, attachIterator)
{
	spl_SplObjectStorage        *intern;
	zval                        *iterator = NULL, *info = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "O|z!", &iterator, zend_ce_iterator, &info) == FAILURE) {
		return;
	}

	intern = Z_SPLOBJSTORAGE_P(getThis());

	if (info != NULL) {
		spl_SplObjectStorageElement *element;

		if (Z_TYPE_P(info) != IS_LONG && Z_TYPE_P(info) != IS_STRING) {
			zend_throw_exception(spl_ce_InvalidArgumentException, "Info must be NULL, integer or string", 0);
			return;
		}

		zend_hash_internal_pointer_reset_ex(&intern->storage, &intern->pos);
		while ((element = zend_hash_get_current_data_ptr_ex(&intern->storage, &intern->pos)) != NULL) {
			if (fast_is_identical_function(info, &element->inf)) {
				zend_throw_exception(spl_ce_InvalidArgumentException, "Key duplication error", 0);
				return;
			}
			zend_hash_move_forward_ex(&intern->storage, &intern->pos);
		}
	}

	spl_object_storage_attach(intern, getThis(), iterator, info);
}
