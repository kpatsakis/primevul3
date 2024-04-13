int spl_object_storage_detach(spl_SplObjectStorage *intern, zval *this, zval *obj) /* {{{ */
{
	int ret = FAILURE;
	zend_string *hash = spl_object_storage_get_hash(intern, this, obj);
	if (!hash) {
		return ret;
	}
	ret = zend_hash_del(&intern->storage, hash);
	spl_object_storage_free_hash(intern, hash);

	return ret;
} /* }}}*/
