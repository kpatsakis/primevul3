smb2_duplicate_extents(const unsigned int xid,
			struct cifsFileInfo *srcfile,
			struct cifsFileInfo *trgtfile, u64 src_off,
			u64 len, u64 dest_off)
{
	int rc;
	unsigned int ret_data_len;
	struct inode *inode;
	struct duplicate_extents_to_file dup_ext_buf;
	struct cifs_tcon *tcon = tlink_tcon(trgtfile->tlink);

	/* server fileays advertise duplicate extent support with this flag */
	if ((le32_to_cpu(tcon->fsAttrInfo.Attributes) &
	     FILE_SUPPORTS_BLOCK_REFCOUNTING) == 0)
		return -EOPNOTSUPP;

	dup_ext_buf.VolatileFileHandle = srcfile->fid.volatile_fid;
	dup_ext_buf.PersistentFileHandle = srcfile->fid.persistent_fid;
	dup_ext_buf.SourceFileOffset = cpu_to_le64(src_off);
	dup_ext_buf.TargetFileOffset = cpu_to_le64(dest_off);
	dup_ext_buf.ByteCount = cpu_to_le64(len);
	cifs_dbg(FYI, "Duplicate extents: src off %lld dst off %lld len %lld\n",
		src_off, dest_off, len);

	inode = d_inode(trgtfile->dentry);
	if (inode->i_size < dest_off + len) {
		rc = smb2_set_file_size(xid, tcon, trgtfile, dest_off + len, false);
		if (rc)
			goto duplicate_extents_out;

		/*
		 * Although also could set plausible allocation size (i_blocks)
		 * here in addition to setting the file size, in reflink
		 * it is likely that the target file is sparse. Its allocation
		 * size will be queried on next revalidate, but it is important
		 * to make sure that file's cached size is updated immediately
		 */
		cifs_setsize(inode, dest_off + len);
	}
	rc = SMB2_ioctl(xid, tcon, trgtfile->fid.persistent_fid,
			trgtfile->fid.volatile_fid,
			FSCTL_DUPLICATE_EXTENTS_TO_FILE,
			true /* is_fsctl */,
			(char *)&dup_ext_buf,
			sizeof(struct duplicate_extents_to_file),
			CIFSMaxBufSize, NULL,
			&ret_data_len);

	if (ret_data_len > 0)
		cifs_dbg(FYI, "Non-zero response length in duplicate extents\n");

duplicate_extents_out:
	return rc;
}