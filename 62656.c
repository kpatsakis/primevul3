static struct dentry *ovl_workdir_create(struct vfsmount *mnt,
					 struct dentry *dentry)
{
	struct inode *dir = dentry->d_inode;
	struct dentry *work;
	int err;
	bool retried = false;

	err = mnt_want_write(mnt);
	if (err)
		return ERR_PTR(err);

	mutex_lock_nested(&dir->i_mutex, I_MUTEX_PARENT);
retry:
	work = lookup_one_len(OVL_WORKDIR_NAME, dentry,
			      strlen(OVL_WORKDIR_NAME));

	if (!IS_ERR(work)) {
		struct kstat stat = {
			.mode = S_IFDIR | 0,
		};

		if (work->d_inode) {
			err = -EEXIST;
			if (retried)
				goto out_dput;

			retried = true;
			ovl_cleanup(dir, work);
			dput(work);
			goto retry;
		}

		err = ovl_create_real(dir, work, &stat, NULL, NULL, true);
		if (err)
			goto out_dput;
	}
out_unlock:
	mutex_unlock(&dir->i_mutex);
	mnt_drop_write(mnt);

	return work;

out_dput:
	dput(work);
	work = ERR_PTR(err);
	goto out_unlock;
}
