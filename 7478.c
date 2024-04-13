smb2_is_status_io_timeout(char *buf)
{
	struct smb2_hdr *shdr = (struct smb2_hdr *)buf;

	if (shdr->Status == STATUS_IO_TIMEOUT)
		return true;
	else
		return false;
}