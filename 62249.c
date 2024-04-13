SPL_METHOD(MultipleIterator, rewind)
{
	spl_SplObjectStorage        *intern;
	spl_SplObjectStorageElement *element;
	zval                        *it;

	intern = Z_SPLOBJSTORAGE_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	zend_hash_internal_pointer_reset_ex(&intern->storage, &intern->pos);
	while ((element = zend_hash_get_current_data_ptr_ex(&intern->storage, &intern->pos)) != NULL && !EG(exception)) {
		it = &element->obj;
		zend_call_method_with_0_params(it, Z_OBJCE_P(it), &Z_OBJCE_P(it)->iterator_funcs.zf_rewind, "rewind", NULL);
		zend_hash_move_forward_ex(&intern->storage, &intern->pos);
	}
}
