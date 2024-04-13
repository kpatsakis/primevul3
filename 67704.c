void git_index__set_ignore_case(git_index *index, bool ignore_case)
{
	index->ignore_case = ignore_case;

	if (ignore_case) {
		index->entries_cmp_path    = git__strcasecmp_cb;
		index->entries_search      = git_index_entry_isrch;
		index->entries_search_path = index_entry_isrch_path;
		index->reuc_search         = reuc_isrch;
	} else {
		index->entries_cmp_path    = git__strcmp_cb;
		index->entries_search      = git_index_entry_srch;
		index->entries_search_path = index_entry_srch_path;
		index->reuc_search         = reuc_srch;
	}

	git_vector_set_cmp(&index->entries,
		ignore_case ? git_index_entry_icmp : git_index_entry_cmp);
	git_vector_sort(&index->entries);

	git_vector_set_cmp(&index->reuc, ignore_case ? reuc_icmp : reuc_cmp);
	git_vector_sort(&index->reuc);
}
