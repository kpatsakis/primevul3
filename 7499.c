smb3_is_transform_hdr(void *buf)
{
	struct smb2_transform_hdr *trhdr = buf;

	return trhdr->ProtocolId == SMB2_TRANSFORM_PROTO_NUM;
}