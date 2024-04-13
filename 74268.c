static int print_cb(
	const git_diff_delta *delta,
	const git_diff_hunk *hunk,
	const git_diff_line *line,
	void *payload)
{
	git_buf *buf = (git_buf *)payload;

	GIT_UNUSED(delta);

	if (hunk)
		git_buf_put(buf, hunk->header, hunk->header_len);

	if (line)
		git_buf_put(buf, line->content, line->content_len);

	return git_buf_oom(buf) ? -1 : 0;
}
