parse_integer(krb5_context context, char **p, const char *file, int lineno,
	      const char *name, heim_integer *integer)
{
    int ret;
    char *p1;
    p1 = strsep(p, " \t");
    if (p1 == NULL) {
	krb5_set_error_message(context, EINVAL,
			       N_("moduli file %s missing %s on line %d", ""),
			       file, name, lineno);
	return EINVAL;
    }
    ret = der_parse_hex_heim_integer(p1, integer);
    if (ret) {
	krb5_set_error_message(context, ret,
			       N_("moduli file %s failed parsing %s "
				  "on line %d", ""),
			       file, name, lineno);
	return ret;
    }

    return 0;
}
