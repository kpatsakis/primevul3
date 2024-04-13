fill_transform_hdr(struct smb2_transform_hdr *tr_hdr, unsigned int orig_len,
		   struct smb_rqst *old_rq, __le16 cipher_type)
{
	struct smb2_hdr *shdr =
			(struct smb2_hdr *)old_rq->rq_iov[0].iov_base;

	memset(tr_hdr, 0, sizeof(struct smb2_transform_hdr));
	tr_hdr->ProtocolId = SMB2_TRANSFORM_PROTO_NUM;
	tr_hdr->OriginalMessageSize = cpu_to_le32(orig_len);
	tr_hdr->Flags = cpu_to_le16(0x01);
	if ((cipher_type == SMB2_ENCRYPTION_AES128_GCM) ||
	    (cipher_type == SMB2_ENCRYPTION_AES256_GCM))
		get_random_bytes(&tr_hdr->Nonce, SMB3_AES_GCM_NONCE);
	else
		get_random_bytes(&tr_hdr->Nonce, SMB3_AES_CCM_NONCE);
	memcpy(&tr_hdr->SessionId, &shdr->SessionId, 8);
}