SPL_METHOD(MultipleIterator, key)
{
	spl_SplObjectStorage *intern;
	intern = Z_SPLOBJSTORAGE_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	spl_multiple_iterator_get_all(intern, SPL_MULTIPLE_ITERATOR_GET_ALL_KEY, return_value);
}
