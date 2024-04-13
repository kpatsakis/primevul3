get_smb2_acl_by_path(struct cifs_sb_info *cifs_sb,
		     const char *path, u32 *pacllen, u32 info)
{
	struct cifs_ntsd *pntsd = NULL;
	u8 oplock = SMB2_OPLOCK_LEVEL_NONE;
	unsigned int xid;
	int rc;
	struct cifs_tcon *tcon;
	struct tcon_link *tlink = cifs_sb_tlink(cifs_sb);
	struct cifs_fid fid;
	struct cifs_open_parms oparms;
	__le16 *utf16_path;

	cifs_dbg(FYI, "get smb3 acl for path %s\n", path);
	if (IS_ERR(tlink))
		return ERR_CAST(tlink);

	tcon = tlink_tcon(tlink);
	xid = get_xid();

	utf16_path = cifs_convert_path_to_utf16(path, cifs_sb);
	if (!utf16_path) {
		rc = -ENOMEM;
		free_xid(xid);
		return ERR_PTR(rc);
	}

	oparms.tcon = tcon;
	oparms.desired_access = READ_CONTROL;
	oparms.disposition = FILE_OPEN;
	/*
	 * When querying an ACL, even if the file is a symlink we want to open
	 * the source not the target, and so the protocol requires that the
	 * client specify this flag when opening a reparse point
	 */
	oparms.create_options = cifs_create_options(cifs_sb, 0) | OPEN_REPARSE_POINT;
	oparms.fid = &fid;
	oparms.reconnect = false;

	if (info & SACL_SECINFO)
		oparms.desired_access |= SYSTEM_SECURITY;

	rc = SMB2_open(xid, &oparms, utf16_path, &oplock, NULL, NULL, NULL,
		       NULL);
	kfree(utf16_path);
	if (!rc) {
		rc = SMB2_query_acl(xid, tlink_tcon(tlink), fid.persistent_fid,
				    fid.volatile_fid, (void **)&pntsd, pacllen,
				    info);
		SMB2_close(xid, tcon, fid.persistent_fid, fid.volatile_fid);
	}

	cifs_put_tlink(tlink);
	free_xid(xid);

	cifs_dbg(FYI, "%s: rc = %d ACL len %d\n", __func__, rc, *pacllen);
	if (rc)
		return ERR_PTR(rc);
	return pntsd;
}