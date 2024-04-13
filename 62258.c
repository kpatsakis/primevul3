static zend_object *spl_object_storage_clone(zval *zobject)
{
	zend_object *old_object;
	zend_object *new_object;

	old_object = Z_OBJ_P(zobject);
	new_object = spl_object_storage_new_ex(old_object->ce, zobject);

	zend_objects_clone_members(new_object, old_object);

	return new_object;
}
