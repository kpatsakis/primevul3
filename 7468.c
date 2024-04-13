crypt_message(struct TCP_Server_Info *server, int num_rqst,
	      struct smb_rqst *rqst, int enc)
{
	struct smb2_transform_hdr *tr_hdr =
		(struct smb2_transform_hdr *)rqst[0].rq_iov[0].iov_base;
	unsigned int assoc_data_len = sizeof(struct smb2_transform_hdr) - 20;
	int rc = 0;
	struct scatterlist *sg;
	u8 sign[SMB2_SIGNATURE_SIZE] = {};
	u8 key[SMB3_ENC_DEC_KEY_SIZE];
	struct aead_request *req;
	char *iv;
	unsigned int iv_len;
	DECLARE_CRYPTO_WAIT(wait);
	struct crypto_aead *tfm;
	unsigned int crypt_len = le32_to_cpu(tr_hdr->OriginalMessageSize);

	rc = smb2_get_enc_key(server, le64_to_cpu(tr_hdr->SessionId), enc, key);
	if (rc) {
		cifs_server_dbg(VFS, "%s: Could not get %scryption key\n", __func__,
			 enc ? "en" : "de");
		return rc;
	}

	rc = smb3_crypto_aead_allocate(server);
	if (rc) {
		cifs_server_dbg(VFS, "%s: crypto alloc failed\n", __func__);
		return rc;
	}

	tfm = enc ? server->secmech.ccmaesencrypt :
						server->secmech.ccmaesdecrypt;

	if ((server->cipher_type == SMB2_ENCRYPTION_AES256_CCM) ||
		(server->cipher_type == SMB2_ENCRYPTION_AES256_GCM))
		rc = crypto_aead_setkey(tfm, key, SMB3_GCM256_CRYPTKEY_SIZE);
	else
		rc = crypto_aead_setkey(tfm, key, SMB3_GCM128_CRYPTKEY_SIZE);

	if (rc) {
		cifs_server_dbg(VFS, "%s: Failed to set aead key %d\n", __func__, rc);
		return rc;
	}

	rc = crypto_aead_setauthsize(tfm, SMB2_SIGNATURE_SIZE);
	if (rc) {
		cifs_server_dbg(VFS, "%s: Failed to set authsize %d\n", __func__, rc);
		return rc;
	}

	req = aead_request_alloc(tfm, GFP_KERNEL);
	if (!req) {
		cifs_server_dbg(VFS, "%s: Failed to alloc aead request\n", __func__);
		return -ENOMEM;
	}

	if (!enc) {
		memcpy(sign, &tr_hdr->Signature, SMB2_SIGNATURE_SIZE);
		crypt_len += SMB2_SIGNATURE_SIZE;
	}

	sg = init_sg(num_rqst, rqst, sign);
	if (!sg) {
		cifs_server_dbg(VFS, "%s: Failed to init sg\n", __func__);
		rc = -ENOMEM;
		goto free_req;
	}

	iv_len = crypto_aead_ivsize(tfm);
	iv = kzalloc(iv_len, GFP_KERNEL);
	if (!iv) {
		cifs_server_dbg(VFS, "%s: Failed to alloc iv\n", __func__);
		rc = -ENOMEM;
		goto free_sg;
	}

	if ((server->cipher_type == SMB2_ENCRYPTION_AES128_GCM) ||
	    (server->cipher_type == SMB2_ENCRYPTION_AES256_GCM))
		memcpy(iv, (char *)tr_hdr->Nonce, SMB3_AES_GCM_NONCE);
	else {
		iv[0] = 3;
		memcpy(iv + 1, (char *)tr_hdr->Nonce, SMB3_AES_CCM_NONCE);
	}

	aead_request_set_crypt(req, sg, sg, crypt_len, iv);
	aead_request_set_ad(req, assoc_data_len);

	aead_request_set_callback(req, CRYPTO_TFM_REQ_MAY_BACKLOG,
				  crypto_req_done, &wait);

	rc = crypto_wait_req(enc ? crypto_aead_encrypt(req)
				: crypto_aead_decrypt(req), &wait);

	if (!rc && enc)
		memcpy(&tr_hdr->Signature, sign, SMB2_SIGNATURE_SIZE);

	kfree(iv);
free_sg:
	kfree(sg);
free_req:
	kfree(req);
	return rc;
}