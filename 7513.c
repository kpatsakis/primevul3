set_smb2_acl(struct cifs_ntsd *pnntsd, __u32 acllen,
		struct inode *inode, const char *path, int aclflag)
{
	u8 oplock = SMB2_OPLOCK_LEVEL_NONE;
	unsigned int xid;
	int rc, access_flags = 0;
	struct cifs_tcon *tcon;
	struct cifs_sb_info *cifs_sb = CIFS_SB(inode->i_sb);
	struct tcon_link *tlink = cifs_sb_tlink(cifs_sb);
	struct cifs_fid fid;
	struct cifs_open_parms oparms;
	__le16 *utf16_path;

	cifs_dbg(FYI, "set smb3 acl for path %s\n", path);
	if (IS_ERR(tlink))
		return PTR_ERR(tlink);

	tcon = tlink_tcon(tlink);
	xid = get_xid();

	if (aclflag & CIFS_ACL_OWNER || aclflag & CIFS_ACL_GROUP)
		access_flags |= WRITE_OWNER;
	if (aclflag & CIFS_ACL_SACL)
		access_flags |= SYSTEM_SECURITY;
	if (aclflag & CIFS_ACL_DACL)
		access_flags |= WRITE_DAC;

	utf16_path = cifs_convert_path_to_utf16(path, cifs_sb);
	if (!utf16_path) {
		rc = -ENOMEM;
		free_xid(xid);
		return rc;
	}

	oparms.tcon = tcon;
	oparms.desired_access = access_flags;
	oparms.create_options = cifs_create_options(cifs_sb, 0);
	oparms.disposition = FILE_OPEN;
	oparms.path = path;
	oparms.fid = &fid;
	oparms.reconnect = false;

	rc = SMB2_open(xid, &oparms, utf16_path, &oplock, NULL, NULL,
		       NULL, NULL);
	kfree(utf16_path);
	if (!rc) {
		rc = SMB2_set_acl(xid, tlink_tcon(tlink), fid.persistent_fid,
			    fid.volatile_fid, pnntsd, acllen, aclflag);
		SMB2_close(xid, tcon, fid.persistent_fid, fid.volatile_fid);
	}

	cifs_put_tlink(tlink);
	free_xid(xid);
	return rc;
}