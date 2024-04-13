parse_reparse_symlink(struct reparse_symlink_data_buffer *symlink_buf,
		      u32 plen, char **target_path,
		      struct cifs_sb_info *cifs_sb)
{
	unsigned int sub_len;
	unsigned int sub_offset;

	/* We handle Symbolic Link reparse tag here. See: MS-FSCC 2.1.2.4 */

	sub_offset = le16_to_cpu(symlink_buf->SubstituteNameOffset);
	sub_len = le16_to_cpu(symlink_buf->SubstituteNameLength);
	if (sub_offset + 20 > plen ||
	    sub_offset + sub_len + 20 > plen) {
		cifs_dbg(VFS, "srv returned malformed symlink buffer\n");
		return -EIO;
	}

	*target_path = cifs_strndup_from_utf16(
				symlink_buf->PathBuffer + sub_offset,
				sub_len, true, cifs_sb->local_nls);
	if (!(*target_path))
		return -ENOMEM;

	convert_delimiter(*target_path, '/');
	cifs_dbg(FYI, "%s: target path: %s\n", __func__, *target_path);

	return 0;
}