static int create_name_extension_data(git_buf *name_buf, git_index_name_entry *conflict_name)
{
	int error = 0;

	if (conflict_name->ancestor == NULL)
		error = git_buf_put(name_buf, "\0", 1);
	else
		error = git_buf_put(name_buf, conflict_name->ancestor, strlen(conflict_name->ancestor) + 1);

	if (error != 0)
		goto on_error;

	if (conflict_name->ours == NULL)
		error = git_buf_put(name_buf, "\0", 1);
	else
		error = git_buf_put(name_buf, conflict_name->ours, strlen(conflict_name->ours) + 1);

	if (error != 0)
		goto on_error;

	if (conflict_name->theirs == NULL)
		error = git_buf_put(name_buf, "\0", 1);
	else
		error = git_buf_put(name_buf, conflict_name->theirs, strlen(conflict_name->theirs) + 1);

on_error:
	return error;
}
