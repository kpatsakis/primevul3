smb2_downgrade_oplock(struct TCP_Server_Info *server,
		      struct cifsInodeInfo *cinode, __u32 oplock,
		      unsigned int epoch, bool *purge_cache)
{
	server->ops->set_oplock_level(cinode, oplock, 0, NULL);
}