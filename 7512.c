smb2_is_network_name_deleted(char *buf, struct TCP_Server_Info *server)
{
	struct smb2_hdr *shdr = (struct smb2_hdr *)buf;
	struct list_head *tmp, *tmp1;
	struct cifs_ses *ses;
	struct cifs_tcon *tcon;

	if (shdr->Status != STATUS_NETWORK_NAME_DELETED)
		return;

	spin_lock(&cifs_tcp_ses_lock);
	list_for_each(tmp, &server->smb_ses_list) {
		ses = list_entry(tmp, struct cifs_ses, smb_ses_list);
		list_for_each(tmp1, &ses->tcon_list) {
			tcon = list_entry(tmp1, struct cifs_tcon, tcon_list);
			if (tcon->tid == le32_to_cpu(shdr->Id.SyncId.TreeId)) {
				tcon->need_reconnect = true;
				spin_unlock(&cifs_tcp_ses_lock);
				pr_warn_once("Server share %s deleted.\n",
					     tcon->treeName);
				return;
			}
		}
	}
	spin_unlock(&cifs_tcp_ses_lock);
}