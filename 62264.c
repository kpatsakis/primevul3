static inline spl_SplObjectStorage *spl_object_storage_from_obj(zend_object *obj) /* {{{ */ {
	return (spl_SplObjectStorage*)((char*)(obj) - XtOffsetOf(spl_SplObjectStorage, std));
}
/* }}} */
