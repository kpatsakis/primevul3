static int create_reuc_extension_data(git_buf *reuc_buf, git_index_reuc_entry *reuc)
{
	int i;
	int error = 0;

	if ((error = git_buf_put(reuc_buf, reuc->path, strlen(reuc->path) + 1)) < 0)
		return error;

	for (i = 0; i < 3; i++) {
		if ((error = git_buf_printf(reuc_buf, "%o", reuc->mode[i])) < 0 ||
			(error = git_buf_put(reuc_buf, "\0", 1)) < 0)
			return error;
	}

	for (i = 0; i < 3; i++) {
		if (reuc->mode[i] && (error = git_buf_put(reuc_buf, (char *)&reuc->oid[i].id, GIT_OID_RAWSZ)) < 0)
			return error;
	}

	return 0;
}
