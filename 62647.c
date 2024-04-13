enum ovl_path_type ovl_path_type(struct dentry *dentry)
{
	struct ovl_entry *oe = dentry->d_fsdata;

	if (oe->__upperdentry) {
		if (oe->lowerdentry) {
			if (S_ISDIR(dentry->d_inode->i_mode))
				return OVL_PATH_MERGE;
			else
				return OVL_PATH_UPPER;
		} else {
			if (oe->opaque)
				return OVL_PATH_UPPER;
			else
				return OVL_PATH_PURE_UPPER;
		}
	} else {
		return OVL_PATH_LOWER;
	}
}
