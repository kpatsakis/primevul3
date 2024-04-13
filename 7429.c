smb311_queryfs(const unsigned int xid, struct cifs_tcon *tcon,
	       struct cifs_sb_info *cifs_sb, struct kstatfs *buf)
{
	int rc;
	__le16 srch_path = 0; /* Null - open root of share */
	u8 oplock = SMB2_OPLOCK_LEVEL_NONE;
	struct cifs_open_parms oparms;
	struct cifs_fid fid;

	if (!tcon->posix_extensions)
		return smb2_queryfs(xid, tcon, cifs_sb, buf);

	oparms.tcon = tcon;
	oparms.desired_access = FILE_READ_ATTRIBUTES;
	oparms.disposition = FILE_OPEN;
	oparms.create_options = cifs_create_options(cifs_sb, 0);
	oparms.fid = &fid;
	oparms.reconnect = false;

	rc = SMB2_open(xid, &oparms, &srch_path, &oplock, NULL, NULL,
		       NULL, NULL);
	if (rc)
		return rc;

	rc = SMB311_posix_qfs_info(xid, tcon, fid.persistent_fid,
				   fid.volatile_fid, buf);
	buf->f_type = SMB2_SUPER_MAGIC;
	SMB2_close(xid, tcon, fid.persistent_fid, fid.volatile_fid);
	return rc;
}