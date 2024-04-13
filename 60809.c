static void php_wddx_process_data(void *user_data, const XML_Char *s, int len)
{
	st_entry *ent;
	wddx_stack *stack = (wddx_stack *)user_data;

	if (!wddx_stack_is_empty(stack) && !stack->done) {
		wddx_stack_top(stack, (void**)&ent);
		switch (ent->type) {
			case ST_BINARY:
			case ST_STRING:
				if (Z_STRLEN(ent->data) == 0) {
					zval_ptr_dtor(&ent->data);
					ZVAL_STRINGL(&ent->data, (char *)s, len);
				} else {
					Z_STR(ent->data) = zend_string_extend(Z_STR(ent->data), Z_STRLEN(ent->data) + len, 0);
					memcpy(Z_STRVAL(ent->data) + Z_STRLEN(ent->data) - len, (char *)s, len);
					Z_STRVAL(ent->data)[Z_STRLEN(ent->data)] = '\0';
				}
				break;
			case ST_NUMBER:
				ZVAL_STRINGL(&ent->data, (char *)s, len);
				convert_scalar_to_number(&ent->data);
				break;

			case ST_BOOLEAN:
				if (!strcmp((char *)s, "true")) {
					ZVAL_TRUE(&ent->data);
				} else if (!strcmp((char *)s, "false")) {
					ZVAL_FALSE(&ent->data);
				} else {
					zval_ptr_dtor(&ent->data);
					if (ent->varname) {
						efree(ent->varname);
						ent->varname = NULL;
					}
					ZVAL_UNDEF(&ent->data);
				}
				break;

			case ST_DATETIME: {
				zend_string *str;

				if (Z_TYPE(ent->data) == IS_STRING) {
					str = zend_string_safe_alloc(Z_STRLEN(ent->data), 1, len, 0);
					memcpy(ZSTR_VAL(str), Z_STRVAL(ent->data), Z_STRLEN(ent->data));
					memcpy(ZSTR_VAL(str) + Z_STRLEN(ent->data), s, len);
					ZSTR_VAL(str)[ZSTR_LEN(str)] = '\0';
					zval_dtor(&ent->data);
				} else {
					str = zend_string_init((char *)s, len, 0);
				}

				ZVAL_LONG(&ent->data, php_parse_date(ZSTR_VAL(str), NULL));
				/* date out of range < 1969 or > 2038 */
				if (Z_LVAL(ent->data) == -1) {
					ZVAL_STR_COPY(&ent->data, str);
				}

				zend_string_release(str);
			}
				break;

			default:
				break;
		}
	}
}
