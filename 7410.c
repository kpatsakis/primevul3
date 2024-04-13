smb2_close_cached_fid(struct kref *ref)
{
	struct cached_fid *cfid = container_of(ref, struct cached_fid,
					       refcount);

	if (cfid->is_valid) {
		cifs_dbg(FYI, "clear cached root file handle\n");
		SMB2_close(0, cfid->tcon, cfid->fid->persistent_fid,
			   cfid->fid->volatile_fid);
	}

	/*
	 * We only check validity above to send SMB2_close,
	 * but we still need to invalidate these entries
	 * when this function is called
	 */
	cfid->is_valid = false;
	cfid->file_all_info_is_valid = false;
	cfid->has_lease = false;
	if (cfid->dentry) {
		dput(cfid->dentry);
		cfid->dentry = NULL;
	}
}