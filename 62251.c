SPL_METHOD(MultipleIterator, valid)
{
	spl_SplObjectStorage        *intern;
	spl_SplObjectStorageElement *element;
	zval                        *it, retval;
	zend_long                         expect, valid;

	intern = Z_SPLOBJSTORAGE_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	if (!zend_hash_num_elements(&intern->storage)) {
		RETURN_FALSE;
	}

	expect = (intern->flags & MIT_NEED_ALL) ? 1 : 0;

	zend_hash_internal_pointer_reset_ex(&intern->storage, &intern->pos);
	while ((element = zend_hash_get_current_data_ptr_ex(&intern->storage, &intern->pos)) != NULL && !EG(exception)) {
		it = &element->obj;
		zend_call_method_with_0_params(it, Z_OBJCE_P(it), &Z_OBJCE_P(it)->iterator_funcs.zf_valid, "valid", &retval);

		if (!Z_ISUNDEF(retval)) {
			valid = (Z_TYPE(retval) == IS_TRUE);
			zval_ptr_dtor(&retval);
		} else {
			valid = 0;
		}

		if (expect != valid) {
			RETURN_BOOL(!expect);
		}

		zend_hash_move_forward_ex(&intern->storage, &intern->pos);
	}

	RETURN_BOOL(expect);
}
