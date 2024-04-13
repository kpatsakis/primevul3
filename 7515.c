parse_reparse_point(struct reparse_data_buffer *buf,
		    u32 plen, char **target_path,
		    struct cifs_sb_info *cifs_sb)
{
	if (plen < sizeof(struct reparse_data_buffer)) {
		cifs_dbg(VFS, "reparse buffer is too small. Must be at least 8 bytes but was %d\n",
			 plen);
		return -EIO;
	}

	if (plen < le16_to_cpu(buf->ReparseDataLength) +
	    sizeof(struct reparse_data_buffer)) {
		cifs_dbg(VFS, "srv returned invalid reparse buf length: %d\n",
			 plen);
		return -EIO;
	}

	/* See MS-FSCC 2.1.2 */
	switch (le32_to_cpu(buf->ReparseTag)) {
	case IO_REPARSE_TAG_NFS:
		return parse_reparse_posix(
			(struct reparse_posix_data *)buf,
			plen, target_path, cifs_sb);
	case IO_REPARSE_TAG_SYMLINK:
		return parse_reparse_symlink(
			(struct reparse_symlink_data_buffer *)buf,
			plen, target_path, cifs_sb);
	default:
		cifs_dbg(VFS, "srv returned unknown symlink buffer tag:0x%08x\n",
			 le32_to_cpu(buf->ReparseTag));
		return -EOPNOTSUPP;
	}
}