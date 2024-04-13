SPL_METHOD(MultipleIterator, current)
{
	spl_SplObjectStorage        *intern;
	intern = Z_SPLOBJSTORAGE_P(getThis());

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	spl_multiple_iterator_get_all(intern, SPL_MULTIPLE_ITERATOR_GET_ALL_CURRENT, return_value);
}
