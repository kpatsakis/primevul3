smb2_make_node(unsigned int xid, struct inode *inode,
	       struct dentry *dentry, struct cifs_tcon *tcon,
	       const char *full_path, umode_t mode, dev_t dev)
{
	struct cifs_sb_info *cifs_sb = CIFS_SB(inode->i_sb);
	int rc = -EPERM;
	FILE_ALL_INFO *buf = NULL;
	struct cifs_io_parms io_parms = {0};
	__u32 oplock = 0;
	struct cifs_fid fid;
	struct cifs_open_parms oparms;
	unsigned int bytes_written;
	struct win_dev *pdev;
	struct kvec iov[2];

	/*
	 * Check if mounted with mount parm 'sfu' mount parm.
	 * SFU emulation should work with all servers, but only
	 * supports block and char device (no socket & fifo),
	 * and was used by default in earlier versions of Windows
	 */
	if (!(cifs_sb->mnt_cifs_flags & CIFS_MOUNT_UNX_EMUL))
		goto out;

	/*
	 * TODO: Add ability to create instead via reparse point. Windows (e.g.
	 * their current NFS server) uses this approach to expose special files
	 * over SMB2/SMB3 and Samba will do this with SMB3.1.1 POSIX Extensions
	 */

	if (!S_ISCHR(mode) && !S_ISBLK(mode))
		goto out;

	cifs_dbg(FYI, "sfu compat create special file\n");

	buf = kmalloc(sizeof(FILE_ALL_INFO), GFP_KERNEL);
	if (buf == NULL) {
		rc = -ENOMEM;
		goto out;
	}

	oparms.tcon = tcon;
	oparms.cifs_sb = cifs_sb;
	oparms.desired_access = GENERIC_WRITE;
	oparms.create_options = cifs_create_options(cifs_sb, CREATE_NOT_DIR |
						    CREATE_OPTION_SPECIAL);
	oparms.disposition = FILE_CREATE;
	oparms.path = full_path;
	oparms.fid = &fid;
	oparms.reconnect = false;

	if (tcon->ses->server->oplocks)
		oplock = REQ_OPLOCK;
	else
		oplock = 0;
	rc = tcon->ses->server->ops->open(xid, &oparms, &oplock, buf);
	if (rc)
		goto out;

	/*
	 * BB Do not bother to decode buf since no local inode yet to put
	 * timestamps in, but we can reuse it safely.
	 */

	pdev = (struct win_dev *)buf;
	io_parms.pid = current->tgid;
	io_parms.tcon = tcon;
	io_parms.offset = 0;
	io_parms.length = sizeof(struct win_dev);
	iov[1].iov_base = buf;
	iov[1].iov_len = sizeof(struct win_dev);
	if (S_ISCHR(mode)) {
		memcpy(pdev->type, "IntxCHR", 8);
		pdev->major = cpu_to_le64(MAJOR(dev));
		pdev->minor = cpu_to_le64(MINOR(dev));
		rc = tcon->ses->server->ops->sync_write(xid, &fid, &io_parms,
							&bytes_written, iov, 1);
	} else if (S_ISBLK(mode)) {
		memcpy(pdev->type, "IntxBLK", 8);
		pdev->major = cpu_to_le64(MAJOR(dev));
		pdev->minor = cpu_to_le64(MINOR(dev));
		rc = tcon->ses->server->ops->sync_write(xid, &fid, &io_parms,
							&bytes_written, iov, 1);
	}
	tcon->ses->server->ops->close(xid, tcon, &fid);
	d_drop(dentry);

	/* FIXME: add code here to set EAs */
out:
	kfree(buf);
	return rc;
}