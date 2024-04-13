static HashTable* spl_object_storage_debug_info(zval *obj, int *is_temp) /* {{{ */
{
	spl_SplObjectStorage *intern = Z_SPLOBJSTORAGE_P(obj);
	spl_SplObjectStorageElement *element;
	HashTable *props;
	zval tmp, storage;
	zend_string *md5str;
	zend_string *zname;
	HashTable *debug_info;

	*is_temp = 1;

	props = Z_OBJPROP_P(obj);

	ALLOC_HASHTABLE(debug_info);
	ZEND_INIT_SYMTABLE_EX(debug_info, zend_hash_num_elements(props) + 1, 0);
	zend_hash_copy(debug_info, props, (copy_ctor_func_t)zval_add_ref);

	array_init(&storage);

	ZEND_HASH_FOREACH_PTR(&intern->storage, element) {
		md5str = php_spl_object_hash(&element->obj);
		array_init(&tmp);
		/* Incrementing the refcount of obj and inf would confuse the garbage collector.
		 * Prefer to null the destructor */
		Z_ARRVAL_P(&tmp)->pDestructor = NULL;
		add_assoc_zval_ex(&tmp, "obj", sizeof("obj") - 1, &element->obj);
		add_assoc_zval_ex(&tmp, "inf", sizeof("inf") - 1, &element->inf);
		zend_hash_update(Z_ARRVAL(storage), md5str, &tmp);
		zend_string_release(md5str);
	} ZEND_HASH_FOREACH_END();

	zname = spl_gen_private_prop_name(spl_ce_SplObjectStorage, "storage", sizeof("storage")-1);
	zend_symtable_update(debug_info, zname, &storage);
	zend_string_release(zname);

	return debug_info;
}
/* }}} */
