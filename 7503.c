get_smb2_acl_by_fid(struct cifs_sb_info *cifs_sb,
		    const struct cifs_fid *cifsfid, u32 *pacllen, u32 info)
{
	struct cifs_ntsd *pntsd = NULL;
	unsigned int xid;
	int rc = -EOPNOTSUPP;
	struct tcon_link *tlink = cifs_sb_tlink(cifs_sb);

	if (IS_ERR(tlink))
		return ERR_CAST(tlink);

	xid = get_xid();
	cifs_dbg(FYI, "trying to get acl\n");

	rc = SMB2_query_acl(xid, tlink_tcon(tlink), cifsfid->persistent_fid,
			    cifsfid->volatile_fid, (void **)&pntsd, pacllen,
			    info);
	free_xid(xid);

	cifs_put_tlink(tlink);

	cifs_dbg(FYI, "%s: rc = %d ACL len %d\n", __func__, rc, *pacllen);
	if (rc)
		return ERR_PTR(rc);
	return pntsd;

}