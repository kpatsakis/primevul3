static int fsck_commit_buffer(struct commit *commit, const char *buffer,
	unsigned long size, struct fsck_options *options)
{
	unsigned char tree_sha1[20], sha1[20];
	struct commit_graft *graft;
	unsigned parent_count, parent_line_count = 0, author_count;
	int err;
	const char *buffer_begin = buffer;

	if (verify_headers(buffer, size, &commit->object, options))
		return -1;

	if (!skip_prefix(buffer, "tree ", &buffer))
		return report(options, &commit->object, FSCK_MSG_MISSING_TREE, "invalid format - expected 'tree' line");
	if (get_sha1_hex(buffer, tree_sha1) || buffer[40] != '\n') {
		err = report(options, &commit->object, FSCK_MSG_BAD_TREE_SHA1, "invalid 'tree' line format - bad sha1");
		if (err)
			return err;
	}
	buffer += 41;
	while (skip_prefix(buffer, "parent ", &buffer)) {
		if (get_sha1_hex(buffer, sha1) || buffer[40] != '\n') {
			err = report(options, &commit->object, FSCK_MSG_BAD_PARENT_SHA1, "invalid 'parent' line format - bad sha1");
			if (err)
				return err;
		}
		buffer += 41;
		parent_line_count++;
	}
	graft = lookup_commit_graft(&commit->object.oid);
	parent_count = commit_list_count(commit->parents);
	if (graft) {
		if (graft->nr_parent == -1 && !parent_count)
			; /* shallow commit */
		else if (graft->nr_parent != parent_count) {
			err = report(options, &commit->object, FSCK_MSG_MISSING_GRAFT, "graft objects missing");
			if (err)
				return err;
		}
	} else {
		if (parent_count != parent_line_count) {
			err = report(options, &commit->object, FSCK_MSG_MISSING_PARENT, "parent objects missing");
			if (err)
				return err;
		}
	}
	author_count = 0;
	while (skip_prefix(buffer, "author ", &buffer)) {
		author_count++;
		err = fsck_ident(&buffer, &commit->object, options);
		if (err)
			return err;
	}
	if (author_count < 1)
		err = report(options, &commit->object, FSCK_MSG_MISSING_AUTHOR, "invalid format - expected 'author' line");
	else if (author_count > 1)
		err = report(options, &commit->object, FSCK_MSG_MULTIPLE_AUTHORS, "invalid format - multiple 'author' lines");
	if (err)
		return err;
	if (!skip_prefix(buffer, "committer ", &buffer))
		return report(options, &commit->object, FSCK_MSG_MISSING_COMMITTER, "invalid format - expected 'committer' line");
	err = fsck_ident(&buffer, &commit->object, options);
	if (err)
		return err;
	if (!commit->tree) {
		err = report(options, &commit->object, FSCK_MSG_BAD_TREE, "could not load commit's tree %s", sha1_to_hex(tree_sha1));
		if (err)
			return err;
	}
	if (memchr(buffer_begin, '\0', size)) {
		err = report(options, &commit->object, FSCK_MSG_NUL_IN_COMMIT,
			     "NUL byte in the commit object body");
		if (err)
			return err;
	}
	return 0;
}
