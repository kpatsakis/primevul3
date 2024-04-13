spl_SplObjectStorageElement *spl_object_storage_attach(spl_SplObjectStorage *intern, zval *this, zval *obj, zval *inf) /* {{{ */
{
	spl_SplObjectStorageElement *pelement, element;
	zend_string *hash = spl_object_storage_get_hash(intern, this, obj);

	if (!hash) {
		return NULL;
	}

	pelement = spl_object_storage_get(intern, hash);

	if (pelement) {
		zval_ptr_dtor(&pelement->inf);
		if (inf) {
			ZVAL_COPY(&pelement->inf, inf);
		} else {
			ZVAL_NULL(&pelement->inf);
		}
		spl_object_storage_free_hash(intern, hash);
		return pelement;
	}

	ZVAL_COPY(&element.obj, obj);
	if (inf) {
		ZVAL_COPY(&element.inf, inf);
	} else {
		ZVAL_NULL(&element.inf);
	}
	pelement = zend_hash_update_mem(&intern->storage, hash, &element, sizeof(spl_SplObjectStorageElement));
	spl_object_storage_free_hash(intern, hash);
	return pelement;
} /* }}} */
