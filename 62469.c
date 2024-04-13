static const char* get_option(const char* options, const char* option_name)
{
	const char* p;
	size_t length = strlen(option_name);

	for (p = strstr(options, option_name); p; p = strstr(p + 1, option_name))
		if ((p == options || p[-1] == ',') && p[length] == '=')
			return p + length + 1;
	return NULL;
}
