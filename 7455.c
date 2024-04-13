smb21_is_read_op(__u32 oplock)
{
	return (oplock & SMB2_LEASE_READ_CACHING_HE) &&
	       !(oplock & SMB2_LEASE_WRITE_CACHING_HE);
}