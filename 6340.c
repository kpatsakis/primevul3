check_regexp_table_name(
	std::string tablespace,
	const char* error_context,
	const char* pattern
)
{
	bool result = false;
	get_table_name_from_tablespace(tablespace);
	xb_regex_t preg;
	size_t nmatch = 1;
	xb_regmatch_t pmatch[1];
	compile_regex(pattern, error_context, &preg);

	if (xb_regexec(&preg, tablespace.c_str(), nmatch, pmatch, 0) != REG_NOMATCH) {
		result = true;
	}
	xb_regfree(&preg);
	return result;
}