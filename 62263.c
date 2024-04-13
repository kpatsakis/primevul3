static void spl_object_storage_free_hash(spl_SplObjectStorage *intern, zend_string *hash) {
	zend_string_release(hash);
}
