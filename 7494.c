smb3_downgrade_oplock(struct TCP_Server_Info *server,
		       struct cifsInodeInfo *cinode, __u32 oplock,
		       unsigned int epoch, bool *purge_cache)
{
	unsigned int old_state = cinode->oplock;
	unsigned int old_epoch = cinode->epoch;
	unsigned int new_state;

	if (epoch > old_epoch) {
		smb21_set_oplock_level(cinode, oplock, 0, NULL);
		cinode->epoch = epoch;
	}

	new_state = cinode->oplock;
	*purge_cache = false;

	if ((old_state & CIFS_CACHE_READ_FLG) != 0 &&
	    (new_state & CIFS_CACHE_READ_FLG) == 0)
		*purge_cache = true;
	else if (old_state == new_state && (epoch - old_epoch > 1))
		*purge_cache = true;
}