PHPAPI void php_register_variable_safe(char *var, char *strval, size_t str_len, zval *track_vars_array)
{
	zval new_entry;
	assert(strval != NULL);

	/* Prepare value */
	ZVAL_NEW_STR(&new_entry, zend_string_init(strval, str_len, 0));
	php_register_variable_ex(var, &new_entry, track_vars_array);
}
