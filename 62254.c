static zend_object *spl_SplObjectStorage_new(zend_class_entry *class_type)
{
	return spl_object_storage_new_ex(class_type, NULL);
}
