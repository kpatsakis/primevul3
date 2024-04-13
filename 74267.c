static int hunk_cb(
	const git_diff_delta *delta,
	const git_diff_hunk *hunk,
	void *payload)
{
	GIT_UNUSED(delta);
	GIT_UNUSED(hunk);
	GIT_UNUSED(payload);

	cl_fail("did not expect hunk callback");
	return 0;
}
