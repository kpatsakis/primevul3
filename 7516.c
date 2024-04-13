SMB3_request_interfaces(const unsigned int xid, struct cifs_tcon *tcon)
{
	int rc;
	unsigned int ret_data_len = 0;
	struct network_interface_info_ioctl_rsp *out_buf = NULL;
	struct cifs_server_iface *iface_list;
	size_t iface_count;
	struct cifs_ses *ses = tcon->ses;

	rc = SMB2_ioctl(xid, tcon, NO_FILE_ID, NO_FILE_ID,
			FSCTL_QUERY_NETWORK_INTERFACE_INFO, true /* is_fsctl */,
			NULL /* no data input */, 0 /* no data input */,
			CIFSMaxBufSize, (char **)&out_buf, &ret_data_len);
	if (rc == -EOPNOTSUPP) {
		cifs_dbg(FYI,
			 "server does not support query network interfaces\n");
		goto out;
	} else if (rc != 0) {
		cifs_tcon_dbg(VFS, "error %d on ioctl to get interface list\n", rc);
		goto out;
	}

	rc = parse_server_interfaces(out_buf, ret_data_len,
				     &iface_list, &iface_count);
	if (rc)
		goto out;

	/* sort interfaces from fastest to slowest */
	sort(iface_list, iface_count, sizeof(*iface_list), compare_iface, NULL);

	spin_lock(&ses->iface_lock);
	kfree(ses->iface_list);
	ses->iface_list = iface_list;
	ses->iface_count = iface_count;
	ses->iface_last_update = jiffies;
	spin_unlock(&ses->iface_lock);

out:
	kfree(out_buf);
	return rc;
}