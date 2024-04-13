static int index_entry_reuc_init(git_index_reuc_entry **reuc_out,
	const char *path,
	int ancestor_mode, const git_oid *ancestor_oid,
	int our_mode, const git_oid *our_oid,
	int their_mode, const git_oid *their_oid)
{
	git_index_reuc_entry *reuc = NULL;

	assert(reuc_out && path);

	*reuc_out = reuc = reuc_entry_alloc(path);
	GITERR_CHECK_ALLOC(reuc);

	if ((reuc->mode[0] = ancestor_mode) > 0) {
		assert(ancestor_oid);
		git_oid_cpy(&reuc->oid[0], ancestor_oid);
	}

	if ((reuc->mode[1] = our_mode) > 0) {
		assert(our_oid);
		git_oid_cpy(&reuc->oid[1], our_oid);
	}

	if ((reuc->mode[2] = their_mode) > 0) {
		assert(their_oid);
		git_oid_cpy(&reuc->oid[2], their_oid);
	}

	return 0;
}
