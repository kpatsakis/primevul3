static int fsck_tree(struct tree *item, struct fsck_options *options)
{
	int retval = 0;
	int has_null_sha1 = 0;
	int has_full_path = 0;
	int has_empty_name = 0;
	int has_dot = 0;
	int has_dotdot = 0;
	int has_dotgit = 0;
	int has_zero_pad = 0;
	int has_bad_modes = 0;
	int has_dup_entries = 0;
	int not_properly_sorted = 0;
	struct tree_desc desc;
	unsigned o_mode;
	const char *o_name;

	if (init_tree_desc_gently(&desc, item->buffer, item->size)) {
		retval += report(options, &item->object, FSCK_MSG_BAD_TREE, "cannot be parsed as a tree");
		return retval;
	}

	o_mode = 0;
	o_name = NULL;

	while (desc.size) {
		unsigned mode;
		const char *name;
		const struct object_id *oid;

		oid = tree_entry_extract(&desc, &name, &mode);

		has_null_sha1 |= is_null_oid(oid);
		has_full_path |= !!strchr(name, '/');
		has_empty_name |= !*name;
		has_dot |= !strcmp(name, ".");
		has_dotdot |= !strcmp(name, "..");
		has_dotgit |= is_hfs_dotgit(name) || is_ntfs_dotgit(name);
		has_zero_pad |= *(char *)desc.buffer == '0';

		if (is_hfs_dotgitmodules(name) || is_ntfs_dotgitmodules(name)) {
			if (!S_ISLNK(mode))
				oidset_insert(&gitmodules_found, oid);
			else
				retval += report(options, &item->object,
						 FSCK_MSG_GITMODULES_SYMLINK,
						 ".gitmodules is a symbolic link");
		}

		if (update_tree_entry_gently(&desc)) {
			retval += report(options, &item->object, FSCK_MSG_BAD_TREE, "cannot be parsed as a tree");
			break;
		}

		switch (mode) {
		/*
		 * Standard modes..
		 */
		case S_IFREG | 0755:
		case S_IFREG | 0644:
		case S_IFLNK:
		case S_IFDIR:
		case S_IFGITLINK:
			break;
		/*
		 * This is nonstandard, but we had a few of these
		 * early on when we honored the full set of mode
		 * bits..
		 */
		case S_IFREG | 0664:
			if (!options->strict)
				break;
			/* fallthrough */
		default:
			has_bad_modes = 1;
		}

		if (o_name) {
			switch (verify_ordered(o_mode, o_name, mode, name)) {
			case TREE_UNORDERED:
				not_properly_sorted = 1;
				break;
			case TREE_HAS_DUPS:
				has_dup_entries = 1;
				break;
			default:
				break;
			}
		}

		o_mode = mode;
		o_name = name;
	}

	if (has_null_sha1)
		retval += report(options, &item->object, FSCK_MSG_NULL_SHA1, "contains entries pointing to null sha1");
	if (has_full_path)
		retval += report(options, &item->object, FSCK_MSG_FULL_PATHNAME, "contains full pathnames");
	if (has_empty_name)
		retval += report(options, &item->object, FSCK_MSG_EMPTY_NAME, "contains empty pathname");
	if (has_dot)
		retval += report(options, &item->object, FSCK_MSG_HAS_DOT, "contains '.'");
	if (has_dotdot)
		retval += report(options, &item->object, FSCK_MSG_HAS_DOTDOT, "contains '..'");
	if (has_dotgit)
		retval += report(options, &item->object, FSCK_MSG_HAS_DOTGIT, "contains '.git'");
	if (has_zero_pad)
		retval += report(options, &item->object, FSCK_MSG_ZERO_PADDED_FILEMODE, "contains zero-padded file modes");
	if (has_bad_modes)
		retval += report(options, &item->object, FSCK_MSG_BAD_FILEMODE, "contains bad file modes");
	if (has_dup_entries)
		retval += report(options, &item->object, FSCK_MSG_DUPLICATE_ENTRIES, "contains duplicate file entries");
	if (not_properly_sorted)
		retval += report(options, &item->object, FSCK_MSG_TREE_NOT_SORTED, "not properly sorted");
	return retval;
}
