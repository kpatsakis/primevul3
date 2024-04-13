static zend_object *spl_object_storage_new_ex(zend_class_entry *class_type, zval *orig) /* {{{ */
{
	spl_SplObjectStorage *intern;
	zend_class_entry *parent = class_type;

	intern = emalloc(sizeof(spl_SplObjectStorage) + zend_object_properties_size(parent));
	memset(intern, 0, sizeof(spl_SplObjectStorage) - sizeof(zval));
	intern->pos = HT_INVALID_IDX;

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	zend_hash_init(&intern->storage, 0, NULL, spl_object_storage_dtor, 0);

	intern->std.handlers = &spl_handler_SplObjectStorage;

	while (parent) {
		if (parent == spl_ce_SplObjectStorage) {
			if (class_type != spl_ce_SplObjectStorage) {
				intern->fptr_get_hash = zend_hash_str_find_ptr(&class_type->function_table, "gethash", sizeof("gethash") - 1);
				if (intern->fptr_get_hash->common.scope == spl_ce_SplObjectStorage) {
					intern->fptr_get_hash = NULL;
				}
			}
			break;
		}

		parent = parent->parent;
	}

	if (orig) {
		spl_SplObjectStorage *other = Z_SPLOBJSTORAGE_P(orig);
		spl_object_storage_addall(intern, orig, other);
	}

	return &intern->std;
}
/* }}} */
