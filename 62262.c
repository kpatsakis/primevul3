static void spl_object_storage_dtor(zval *element) /* {{{ */
{
	spl_SplObjectStorageElement *el = Z_PTR_P(element);
	zval_ptr_dtor(&el->obj);
	zval_ptr_dtor(&el->inf);
	efree(el);
} /* }}} */
