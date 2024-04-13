smb3_enum_snapshots(const unsigned int xid, struct cifs_tcon *tcon,
		   struct cifsFileInfo *cfile, void __user *ioc_buf)
{
	char *retbuf = NULL;
	unsigned int ret_data_len = 0;
	int rc;
	u32 max_response_size;
	struct smb_snapshot_array snapshot_in;

	/*
	 * On the first query to enumerate the list of snapshots available
	 * for this volume the buffer begins with 0 (number of snapshots
	 * which can be returned is zero since at that point we do not know
	 * how big the buffer needs to be). On the second query,
	 * it (ret_data_len) is set to number of snapshots so we can
	 * know to set the maximum response size larger (see below).
	 */
	if (get_user(ret_data_len, (unsigned int __user *)ioc_buf))
		return -EFAULT;

	/*
	 * Note that for snapshot queries that servers like Azure expect that
	 * the first query be minimal size (and just used to get the number/size
	 * of previous versions) so response size must be specified as EXACTLY
	 * sizeof(struct snapshot_array) which is 16 when rounded up to multiple
	 * of eight bytes.
	 */
	if (ret_data_len == 0)
		max_response_size = MIN_SNAPSHOT_ARRAY_SIZE;
	else
		max_response_size = CIFSMaxBufSize;

	rc = SMB2_ioctl(xid, tcon, cfile->fid.persistent_fid,
			cfile->fid.volatile_fid,
			FSCTL_SRV_ENUMERATE_SNAPSHOTS,
			true /* is_fsctl */,
			NULL, 0 /* no input data */, max_response_size,
			(char **)&retbuf,
			&ret_data_len);
	cifs_dbg(FYI, "enum snaphots ioctl returned %d and ret buflen is %d\n",
			rc, ret_data_len);
	if (rc)
		return rc;

	if (ret_data_len && (ioc_buf != NULL) && (retbuf != NULL)) {
		/* Fixup buffer */
		if (copy_from_user(&snapshot_in, ioc_buf,
		    sizeof(struct smb_snapshot_array))) {
			rc = -EFAULT;
			kfree(retbuf);
			return rc;
		}

		/*
		 * Check for min size, ie not large enough to fit even one GMT
		 * token (snapshot).  On the first ioctl some users may pass in
		 * smaller size (or zero) to simply get the size of the array
		 * so the user space caller can allocate sufficient memory
		 * and retry the ioctl again with larger array size sufficient
		 * to hold all of the snapshot GMT tokens on the second try.
		 */
		if (snapshot_in.snapshot_array_size < GMT_TOKEN_SIZE)
			ret_data_len = sizeof(struct smb_snapshot_array);

		/*
		 * We return struct SRV_SNAPSHOT_ARRAY, followed by
		 * the snapshot array (of 50 byte GMT tokens) each
		 * representing an available previous version of the data
		 */
		if (ret_data_len > (snapshot_in.snapshot_array_size +
					sizeof(struct smb_snapshot_array)))
			ret_data_len = snapshot_in.snapshot_array_size +
					sizeof(struct smb_snapshot_array);

		if (copy_to_user(ioc_buf, retbuf, ret_data_len))
			rc = -EFAULT;
	}

	kfree(retbuf);
	return rc;
}