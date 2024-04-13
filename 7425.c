smb21_set_oplock_level(struct cifsInodeInfo *cinode, __u32 oplock,
		       unsigned int epoch, bool *purge_cache)
{
	char message[5] = {0};
	unsigned int new_oplock = 0;

	oplock &= 0xFF;
	cinode->lease_granted = true;
	if (oplock == SMB2_OPLOCK_LEVEL_NOCHANGE)
		return;

	/* Check if the server granted an oplock rather than a lease */
	if (oplock & SMB2_OPLOCK_LEVEL_EXCLUSIVE)
		return smb2_set_oplock_level(cinode, oplock, epoch,
					     purge_cache);

	if (oplock & SMB2_LEASE_READ_CACHING_HE) {
		new_oplock |= CIFS_CACHE_READ_FLG;
		strcat(message, "R");
	}
	if (oplock & SMB2_LEASE_HANDLE_CACHING_HE) {
		new_oplock |= CIFS_CACHE_HANDLE_FLG;
		strcat(message, "H");
	}
	if (oplock & SMB2_LEASE_WRITE_CACHING_HE) {
		new_oplock |= CIFS_CACHE_WRITE_FLG;
		strcat(message, "W");
	}
	if (!new_oplock)
		strncpy(message, "None", sizeof(message));

	cinode->oplock = new_oplock;
	cifs_dbg(FYI, "%s Lease granted on inode %p\n", message,
		 &cinode->vfs_inode);
}