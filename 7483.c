smb3_notify(const unsigned int xid, struct file *pfile,
	    void __user *ioc_buf)
{
	struct smb3_notify notify;
	struct dentry *dentry = pfile->f_path.dentry;
	struct inode *inode = file_inode(pfile);
	struct cifs_sb_info *cifs_sb = CIFS_SB(inode->i_sb);
	struct cifs_open_parms oparms;
	struct cifs_fid fid;
	struct cifs_tcon *tcon;
	const unsigned char *path;
	void *page = alloc_dentry_path();
	__le16 *utf16_path = NULL;
	u8 oplock = SMB2_OPLOCK_LEVEL_NONE;
	int rc = 0;

	path = build_path_from_dentry(dentry, page);
	if (IS_ERR(path)) {
		rc = PTR_ERR(path);
		goto notify_exit;
	}

	utf16_path = cifs_convert_path_to_utf16(path, cifs_sb);
	if (utf16_path == NULL) {
		rc = -ENOMEM;
		goto notify_exit;
	}

	if (copy_from_user(&notify, ioc_buf, sizeof(struct smb3_notify))) {
		rc = -EFAULT;
		goto notify_exit;
	}

	tcon = cifs_sb_master_tcon(cifs_sb);
	oparms.tcon = tcon;
	oparms.desired_access = FILE_READ_ATTRIBUTES | FILE_READ_DATA;
	oparms.disposition = FILE_OPEN;
	oparms.create_options = cifs_create_options(cifs_sb, 0);
	oparms.fid = &fid;
	oparms.reconnect = false;

	rc = SMB2_open(xid, &oparms, utf16_path, &oplock, NULL, NULL, NULL,
		       NULL);
	if (rc)
		goto notify_exit;

	rc = SMB2_change_notify(xid, tcon, fid.persistent_fid, fid.volatile_fid,
				notify.watch_tree, notify.completion_filter);

	SMB2_close(xid, tcon, fid.persistent_fid, fid.volatile_fid);

	cifs_dbg(FYI, "change notify for path %s rc %d\n", path, rc);

notify_exit:
	free_dentry_path(page);
	kfree(utf16_path);
	return rc;
}