smb2_read_data_length(char *buf, bool in_remaining)
{
	struct smb2_read_rsp *rsp = (struct smb2_read_rsp *)buf;

	if (in_remaining)
		return le32_to_cpu(rsp->DataRemaining);

	return le32_to_cpu(rsp->DataLength);
}