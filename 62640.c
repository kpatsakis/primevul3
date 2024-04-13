struct dentry *ovl_lookup(struct inode *dir, struct dentry *dentry,
			  unsigned int flags)
{
	struct ovl_entry *oe;
	struct dentry *upperdir;
	struct dentry *lowerdir;
	struct dentry *upperdentry = NULL;
	struct dentry *lowerdentry = NULL;
	struct inode *inode = NULL;
	int err;

	err = -ENOMEM;
	oe = ovl_alloc_entry();
	if (!oe)
		goto out;

	upperdir = ovl_dentry_upper(dentry->d_parent);
	lowerdir = ovl_dentry_lower(dentry->d_parent);

	if (upperdir) {
		upperdentry = ovl_lookup_real(upperdir, &dentry->d_name);
		err = PTR_ERR(upperdentry);
		if (IS_ERR(upperdentry))
			goto out_put_dir;

		if (lowerdir && upperdentry) {
			if (ovl_is_whiteout(upperdentry)) {
				dput(upperdentry);
				upperdentry = NULL;
				oe->opaque = true;
			} else if (ovl_is_opaquedir(upperdentry)) {
				oe->opaque = true;
			}
		}
	}
	if (lowerdir && !oe->opaque) {
		lowerdentry = ovl_lookup_real(lowerdir, &dentry->d_name);
		err = PTR_ERR(lowerdentry);
		if (IS_ERR(lowerdentry))
			goto out_dput_upper;
	}

	if (lowerdentry && upperdentry &&
	    (!S_ISDIR(upperdentry->d_inode->i_mode) ||
	     !S_ISDIR(lowerdentry->d_inode->i_mode))) {
		dput(lowerdentry);
		lowerdentry = NULL;
		oe->opaque = true;
	}

	if (lowerdentry || upperdentry) {
		struct dentry *realdentry;

		realdentry = upperdentry ? upperdentry : lowerdentry;
		err = -ENOMEM;
		inode = ovl_new_inode(dentry->d_sb, realdentry->d_inode->i_mode,
				      oe);
		if (!inode)
			goto out_dput;
		ovl_copyattr(realdentry->d_inode, inode);
	}

	oe->__upperdentry = upperdentry;
	oe->lowerdentry = lowerdentry;

	dentry->d_fsdata = oe;
	d_add(dentry, inode);

	return NULL;

out_dput:
	dput(lowerdentry);
out_dput_upper:
	dput(upperdentry);
out_put_dir:
	kfree(oe);
out:
	return ERR_PTR(err);
}
