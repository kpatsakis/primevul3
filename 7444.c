smb2_next_header(char *buf)
{
	struct smb2_hdr *hdr = (struct smb2_hdr *)buf;
	struct smb2_transform_hdr *t_hdr = (struct smb2_transform_hdr *)buf;

	if (hdr->ProtocolId == SMB2_TRANSFORM_PROTO_NUM)
		return sizeof(struct smb2_transform_hdr) +
		  le32_to_cpu(t_hdr->OriginalMessageSize);

	return le32_to_cpu(hdr->NextCommand);
}