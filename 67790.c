static void index_name_entry_free(git_index_name_entry *ne)
{
	if (!ne)
		return;
	git__free(ne->ancestor);
	git__free(ne->ours);
	git__free(ne->theirs);
	git__free(ne);
}
