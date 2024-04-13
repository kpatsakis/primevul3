smb2_read_data_offset(char *buf)
{
	struct smb2_read_rsp *rsp = (struct smb2_read_rsp *)buf;

	return rsp->DataOffset;
}