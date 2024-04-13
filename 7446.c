get_smb2_acl(struct cifs_sb_info *cifs_sb,
	     struct inode *inode, const char *path,
	     u32 *pacllen, u32 info)
{
	struct cifs_ntsd *pntsd = NULL;
	struct cifsFileInfo *open_file = NULL;

	if (inode && !(info & SACL_SECINFO))
		open_file = find_readable_file(CIFS_I(inode), true);
	if (!open_file || (info & SACL_SECINFO))
		return get_smb2_acl_by_path(cifs_sb, path, pacllen, info);

	pntsd = get_smb2_acl_by_fid(cifs_sb, &open_file->fid, pacllen, info);
	cifsFileInfo_put(open_file);
	return pntsd;
}