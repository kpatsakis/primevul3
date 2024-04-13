static int get_int_option(const char* options, const char* option_name,
		int base, int default_value)
{
	const char* p = get_option(options, option_name);

	if (p == NULL)
		return default_value;
	return strtol(p, NULL, base);
}
