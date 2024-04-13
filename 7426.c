smb2_is_read_op(__u32 oplock)
{
	return oplock == SMB2_OPLOCK_LEVEL_II;
}