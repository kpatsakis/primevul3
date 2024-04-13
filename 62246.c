SPL_METHOD(MultipleIterator, getFlags)
{
	spl_SplObjectStorage *intern = Z_SPLOBJSTORAGE_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	RETURN_LONG(intern->flags);
}
