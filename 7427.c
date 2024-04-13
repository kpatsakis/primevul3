smb2_get_dfs_refer(const unsigned int xid, struct cifs_ses *ses,
		   const char *search_name,
		   struct dfs_info3_param **target_nodes,
		   unsigned int *num_of_nodes,
		   const struct nls_table *nls_codepage, int remap)
{
	int rc;
	__le16 *utf16_path = NULL;
	int utf16_path_len = 0;
	struct cifs_tcon *tcon;
	struct fsctl_get_dfs_referral_req *dfs_req = NULL;
	struct get_dfs_referral_rsp *dfs_rsp = NULL;
	u32 dfs_req_size = 0, dfs_rsp_size = 0;
	int retry_count = 0;

	cifs_dbg(FYI, "%s: path: %s\n", __func__, search_name);

	/*
	 * Try to use the IPC tcon, otherwise just use any
	 */
	tcon = ses->tcon_ipc;
	if (tcon == NULL) {
		spin_lock(&cifs_tcp_ses_lock);
		tcon = list_first_entry_or_null(&ses->tcon_list,
						struct cifs_tcon,
						tcon_list);
		if (tcon)
			tcon->tc_count++;
		spin_unlock(&cifs_tcp_ses_lock);
	}

	if (tcon == NULL) {
		cifs_dbg(VFS, "session %p has no tcon available for a dfs referral request\n",
			 ses);
		rc = -ENOTCONN;
		goto out;
	}

	utf16_path = cifs_strndup_to_utf16(search_name, PATH_MAX,
					   &utf16_path_len,
					   nls_codepage, remap);
	if (!utf16_path) {
		rc = -ENOMEM;
		goto out;
	}

	dfs_req_size = sizeof(*dfs_req) + utf16_path_len;
	dfs_req = kzalloc(dfs_req_size, GFP_KERNEL);
	if (!dfs_req) {
		rc = -ENOMEM;
		goto out;
	}

	/* Highest DFS referral version understood */
	dfs_req->MaxReferralLevel = DFS_VERSION;

	/* Path to resolve in an UTF-16 null-terminated string */
	memcpy(dfs_req->RequestFileName, utf16_path, utf16_path_len);

	do {
		rc = SMB2_ioctl(xid, tcon, NO_FILE_ID, NO_FILE_ID,
				FSCTL_DFS_GET_REFERRALS,
				true /* is_fsctl */,
				(char *)dfs_req, dfs_req_size, CIFSMaxBufSize,
				(char **)&dfs_rsp, &dfs_rsp_size);
		if (!is_retryable_error(rc))
			break;
		usleep_range(512, 2048);
	} while (++retry_count < 5);

	if (rc) {
		if (!is_retryable_error(rc) && rc != -ENOENT && rc != -EOPNOTSUPP)
			cifs_tcon_dbg(VFS, "%s: ioctl error: rc=%d\n", __func__, rc);
		goto out;
	}

	rc = parse_dfs_referrals(dfs_rsp, dfs_rsp_size,
				 num_of_nodes, target_nodes,
				 nls_codepage, remap, search_name,
				 true /* is_unicode */);
	if (rc) {
		cifs_tcon_dbg(VFS, "parse error in %s rc=%d\n", __func__, rc);
		goto out;
	}

 out:
	if (tcon && !tcon->ipc) {
		/* ipc tcons are not refcounted */
		spin_lock(&cifs_tcp_ses_lock);
		tcon->tc_count--;
		/* tc_count can never go negative */
		WARN_ON(tcon->tc_count < 0);
		spin_unlock(&cifs_tcp_ses_lock);
	}
	kfree(utf16_path);
	kfree(dfs_req);
	kfree(dfs_rsp);
	return rc;
}