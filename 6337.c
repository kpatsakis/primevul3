is_tmp_table(const std::string &tablespace)
{
	const char* pattern = "^#sql";
	const char* error_context = "is_tmp_table";
	return check_regexp_table_name(tablespace, error_context, pattern);
}