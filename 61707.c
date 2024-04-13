PHPAPI void php_var_dump(zval *struc, int level) /* {{{ */
{
	HashTable *myht;
	zend_string *class_name;
	int is_temp;
	int is_ref = 0;
	zend_ulong num;
	zend_string *key;
	zval *val;
	uint32_t count;

	if (level > 1) {
		php_printf("%*c", level - 1, ' ');
	}

again:
	switch (Z_TYPE_P(struc)) {
		case IS_FALSE:
			php_printf("%sbool(false)\n", COMMON);
			break;
		case IS_TRUE:
			php_printf("%sbool(true)\n", COMMON);
			break;
		case IS_NULL:
			php_printf("%sNULL\n", COMMON);
			break;
		case IS_LONG:
			php_printf("%sint(" ZEND_LONG_FMT ")\n", COMMON, Z_LVAL_P(struc));
			break;
		case IS_DOUBLE:
			php_printf("%sfloat(%.*G)\n", COMMON, (int) EG(precision), Z_DVAL_P(struc));
			break;
		case IS_STRING:
			php_printf("%sstring(%zd) \"", COMMON, Z_STRLEN_P(struc));
			PHPWRITE(Z_STRVAL_P(struc), Z_STRLEN_P(struc));
			PUTS("\"\n");
			break;
		case IS_ARRAY:
			myht = Z_ARRVAL_P(struc);
			if (level > 1 && ZEND_HASH_APPLY_PROTECTION(myht) && ++myht->u.v.nApplyCount > 1) {
				PUTS("*RECURSION*\n");
				--myht->u.v.nApplyCount;
				return;
			}
			count = zend_array_count(myht);
			php_printf("%sarray(%d) {\n", COMMON, count);
			is_temp = 0;

			ZEND_HASH_FOREACH_KEY_VAL_IND(myht, num, key, val) {
				php_array_element_dump(val, num, key, level);
			} ZEND_HASH_FOREACH_END();
			if (level > 1 && ZEND_HASH_APPLY_PROTECTION(myht)) {
				--myht->u.v.nApplyCount;
			}
			if (is_temp) {
				zend_hash_destroy(myht);
				efree(myht);
			}
			if (level > 1) {
				php_printf("%*c", level-1, ' ');
			}
			PUTS("}\n");
			break;
		case IS_OBJECT:
			if (Z_OBJ_APPLY_COUNT_P(struc) > 0) {
				PUTS("*RECURSION*\n");
				return;
			}
			Z_OBJ_INC_APPLY_COUNT_P(struc);

			myht = Z_OBJDEBUG_P(struc, is_temp);
			class_name = Z_OBJ_HANDLER_P(struc, get_class_name)(Z_OBJ_P(struc));
			php_printf("%sobject(%s)#%d (%d) {\n", COMMON, ZSTR_VAL(class_name), Z_OBJ_HANDLE_P(struc), myht ? zend_array_count(myht) : 0);
			zend_string_release(class_name);

			if (myht) {
				zend_ulong num;
				zend_string *key;
				zval *val;

				ZEND_HASH_FOREACH_KEY_VAL_IND(myht, num, key, val) {
					php_object_property_dump(val, num, key, level);
				} ZEND_HASH_FOREACH_END();
				if (is_temp) {
					zend_hash_destroy(myht);
					efree(myht);
				}
			}
			if (level > 1) {
				php_printf("%*c", level-1, ' ');
			}
			PUTS("}\n");
			Z_OBJ_DEC_APPLY_COUNT_P(struc);
			break;
		case IS_RESOURCE: {
			const char *type_name = zend_rsrc_list_get_rsrc_type(Z_RES_P(struc));
			php_printf("%sresource(%pd) of type (%s)\n", COMMON, Z_RES_P(struc)->handle, type_name ? type_name : "Unknown");
			break;
		}
		case IS_REFERENCE:
			if (Z_REFCOUNT_P(struc) > 1) {
				is_ref = 1;
			}
			struc = Z_REFVAL_P(struc);
			goto again;
			break;
		default:
			php_printf("%sUNKNOWN:0\n", COMMON);
			break;
	}
}
/* }}} */
