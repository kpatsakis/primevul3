is_fts_index(const std::string &tablespace)
{
	const char *pattern =
			"^(FTS|fts)_[0-9a-fA-f]{16}_(([0-9a-fA-]{16}_(INDEX|index)_[1-6])|"
			"DELETED_CACHE|deleted_cache|DELETED|deleted|CONFIG|config|BEING_DELETED|"
			"being_deleted|BEING_DELETED_CACHE|beign_deleted_cache)$";
	const char *error_context = "is_fts_index";
	return check_regexp_table_name(tablespace, error_context, pattern);
}