smb2_get_enc_key(struct TCP_Server_Info *server, __u64 ses_id, int enc, u8 *key)
{
	struct cifs_ses *ses;
	u8 *ses_enc_key;

	spin_lock(&cifs_tcp_ses_lock);
	list_for_each_entry(server, &cifs_tcp_ses_list, tcp_ses_list) {
		list_for_each_entry(ses, &server->smb_ses_list, smb_ses_list) {
			if (ses->Suid == ses_id) {
				ses_enc_key = enc ? ses->smb3encryptionkey :
					ses->smb3decryptionkey;
				memcpy(key, ses_enc_key, SMB3_ENC_DEC_KEY_SIZE);
				spin_unlock(&cifs_tcp_ses_lock);
				return 0;
			}
		}
	}
	spin_unlock(&cifs_tcp_ses_lock);

	return -EAGAIN;
}