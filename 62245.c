SPL_METHOD(MultipleIterator, __construct)
{
	spl_SplObjectStorage   *intern;
	zend_long               flags = MIT_NEED_ALL|MIT_KEYS_NUMERIC;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "|l", &flags) == FAILURE) {
		return;
	}

	intern = Z_SPLOBJSTORAGE_P(getThis());
	intern->flags = flags;
}
