parse_reparse_posix(struct reparse_posix_data *symlink_buf,
		      u32 plen, char **target_path,
		      struct cifs_sb_info *cifs_sb)
{
	unsigned int len;

	/* See MS-FSCC 2.1.2.6 for the 'NFS' style reparse tags */
	len = le16_to_cpu(symlink_buf->ReparseDataLength);

	if (le64_to_cpu(symlink_buf->InodeType) != NFS_SPECFILE_LNK) {
		cifs_dbg(VFS, "%lld not a supported symlink type\n",
			le64_to_cpu(symlink_buf->InodeType));
		return -EOPNOTSUPP;
	}

	*target_path = cifs_strndup_from_utf16(
				symlink_buf->PathBuffer,
				len, true, cifs_sb->local_nls);
	if (!(*target_path))
		return -ENOMEM;

	convert_delimiter(*target_path, '/');
	cifs_dbg(FYI, "%s: target path: %s\n", __func__, *target_path);

	return 0;
}