static int fsck_commit(struct commit *commit, const char *data,
	unsigned long size, struct fsck_options *options)
{
	const char *buffer = data ?  data : get_commit_buffer(commit, &size);
	int ret = fsck_commit_buffer(commit, buffer, size, options);
	if (!data)
		unuse_commit_buffer(commit, buffer);
	return ret;
}
