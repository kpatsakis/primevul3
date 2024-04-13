smb2_qfs_tcon(const unsigned int xid, struct cifs_tcon *tcon,
	      struct cifs_sb_info *cifs_sb)
{
	int rc;
	__le16 srch_path = 0; /* Null - open root of share */
	u8 oplock = SMB2_OPLOCK_LEVEL_NONE;
	struct cifs_open_parms oparms;
	struct cifs_fid fid;

	oparms.tcon = tcon;
	oparms.desired_access = FILE_READ_ATTRIBUTES;
	oparms.disposition = FILE_OPEN;
	oparms.create_options = cifs_create_options(cifs_sb, 0);
	oparms.fid = &fid;
	oparms.reconnect = false;

	rc = SMB2_open(xid, &oparms, &srch_path, &oplock, NULL, NULL,
		       NULL, NULL);
	if (rc)
		return;

	SMB2_QFS_attr(xid, tcon, fid.persistent_fid, fid.volatile_fid,
			FS_ATTRIBUTE_INFORMATION);
	SMB2_QFS_attr(xid, tcon, fid.persistent_fid, fid.volatile_fid,
			FS_DEVICE_INFORMATION);
	SMB2_close(xid, tcon, fid.persistent_fid, fid.volatile_fid);
}