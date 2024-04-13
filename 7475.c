smb3_set_oplock_level(struct cifsInodeInfo *cinode, __u32 oplock,
		      unsigned int epoch, bool *purge_cache)
{
	unsigned int old_oplock = cinode->oplock;

	smb21_set_oplock_level(cinode, oplock, epoch, purge_cache);

	if (purge_cache) {
		*purge_cache = false;
		if (old_oplock == CIFS_CACHE_READ_FLG) {
			if (cinode->oplock == CIFS_CACHE_READ_FLG &&
			    (epoch - cinode->epoch > 0))
				*purge_cache = true;
			else if (cinode->oplock == CIFS_CACHE_RH_FLG &&
				 (epoch - cinode->epoch > 1))
				*purge_cache = true;
			else if (cinode->oplock == CIFS_CACHE_RHW_FLG &&
				 (epoch - cinode->epoch > 1))
				*purge_cache = true;
			else if (cinode->oplock == 0 &&
				 (epoch - cinode->epoch > 0))
				*purge_cache = true;
		} else if (old_oplock == CIFS_CACHE_RH_FLG) {
			if (cinode->oplock == CIFS_CACHE_RH_FLG &&
			    (epoch - cinode->epoch > 0))
				*purge_cache = true;
			else if (cinode->oplock == CIFS_CACHE_RHW_FLG &&
				 (epoch - cinode->epoch > 1))
				*purge_cache = true;
		}
		cinode->epoch = epoch;
	}
}