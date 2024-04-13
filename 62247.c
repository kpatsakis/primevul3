SPL_METHOD(MultipleIterator, setFlags)
{
	spl_SplObjectStorage *intern;
	intern = Z_SPLOBJSTORAGE_P(getThis());

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &intern->flags) == FAILURE) {
		return;
	}
}
