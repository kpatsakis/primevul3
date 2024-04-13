void spl_object_storage_addall(spl_SplObjectStorage *intern, zval *this, spl_SplObjectStorage *other) { /* {{{ */
	spl_SplObjectStorageElement *element;

	ZEND_HASH_FOREACH_PTR(&other->storage, element) {
		spl_object_storage_attach(intern, this, &element->obj, &element->inf);
	} ZEND_HASH_FOREACH_END();

	intern->index = 0;
} /* }}} */
