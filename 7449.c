map_oplock_to_lease(u8 oplock)
{
	if (oplock == SMB2_OPLOCK_LEVEL_EXCLUSIVE)
		return SMB2_LEASE_WRITE_CACHING_LE | SMB2_LEASE_READ_CACHING_LE;
	else if (oplock == SMB2_OPLOCK_LEVEL_II)
		return SMB2_LEASE_READ_CACHING_LE;
	else if (oplock == SMB2_OPLOCK_LEVEL_BATCH)
		return SMB2_LEASE_HANDLE_CACHING_LE | SMB2_LEASE_READ_CACHING_LE |
		       SMB2_LEASE_WRITE_CACHING_LE;
	return 0;
}