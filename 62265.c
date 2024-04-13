static HashTable *spl_object_storage_get_gc(zval *obj, zval **table, int *n) /* {{{ */
{
	int i = 0;
	spl_SplObjectStorage *intern = Z_SPLOBJSTORAGE_P(obj);
	spl_SplObjectStorageElement *element;

	if (intern->storage.nNumOfElements * 2 > intern->gcdata_num) {
		intern->gcdata_num = intern->storage.nNumOfElements * 2;
		intern->gcdata = (zval*)erealloc(intern->gcdata, sizeof(zval) * intern->gcdata_num);
	}

	ZEND_HASH_FOREACH_PTR(&intern->storage, element) {
		ZVAL_COPY_VALUE(&intern->gcdata[i++], &element->obj);
		ZVAL_COPY_VALUE(&intern->gcdata[i++], &element->inf);
	} ZEND_HASH_FOREACH_END();

	*table = intern->gcdata;
	*n = i;

	return std_object_handlers.get_properties(obj);
}
/* }}} */
