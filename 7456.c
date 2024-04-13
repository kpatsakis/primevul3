SMB2_request_res_key(const unsigned int xid, struct cifs_tcon *tcon,
		     u64 persistent_fid, u64 volatile_fid,
		     struct copychunk_ioctl *pcchunk)
{
	int rc;
	unsigned int ret_data_len;
	struct resume_key_req *res_key;

	rc = SMB2_ioctl(xid, tcon, persistent_fid, volatile_fid,
			FSCTL_SRV_REQUEST_RESUME_KEY, true /* is_fsctl */,
			NULL, 0 /* no input */, CIFSMaxBufSize,
			(char **)&res_key, &ret_data_len);

	if (rc == -EOPNOTSUPP) {
		pr_warn_once("Server share %s does not support copy range\n", tcon->treeName);
		goto req_res_key_exit;
	} else if (rc) {
		cifs_tcon_dbg(VFS, "refcpy ioctl error %d getting resume key\n", rc);
		goto req_res_key_exit;
	}
	if (ret_data_len < sizeof(struct resume_key_req)) {
		cifs_tcon_dbg(VFS, "Invalid refcopy resume key length\n");
		rc = -EINVAL;
		goto req_res_key_exit;
	}
	memcpy(pcchunk->SourceKey, res_key->ResumeKey, COPY_CHUNK_RES_KEY_SIZE);

req_res_key_exit:
	kfree(res_key);
	return rc;
}