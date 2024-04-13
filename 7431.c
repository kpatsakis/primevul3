smb2_print_stats(struct seq_file *m, struct cifs_tcon *tcon)
{
	atomic_t *sent = tcon->stats.smb2_stats.smb2_com_sent;
	atomic_t *failed = tcon->stats.smb2_stats.smb2_com_failed;

	/*
	 *  Can't display SMB2_NEGOTIATE, SESSION_SETUP, LOGOFF, CANCEL and ECHO
	 *  totals (requests sent) since those SMBs are per-session not per tcon
	 */
	seq_printf(m, "\nBytes read: %llu  Bytes written: %llu",
		   (long long)(tcon->bytes_read),
		   (long long)(tcon->bytes_written));
	seq_printf(m, "\nOpen files: %d total (local), %d open on server",
		   atomic_read(&tcon->num_local_opens),
		   atomic_read(&tcon->num_remote_opens));
	seq_printf(m, "\nTreeConnects: %d total %d failed",
		   atomic_read(&sent[SMB2_TREE_CONNECT_HE]),
		   atomic_read(&failed[SMB2_TREE_CONNECT_HE]));
	seq_printf(m, "\nTreeDisconnects: %d total %d failed",
		   atomic_read(&sent[SMB2_TREE_DISCONNECT_HE]),
		   atomic_read(&failed[SMB2_TREE_DISCONNECT_HE]));
	seq_printf(m, "\nCreates: %d total %d failed",
		   atomic_read(&sent[SMB2_CREATE_HE]),
		   atomic_read(&failed[SMB2_CREATE_HE]));
	seq_printf(m, "\nCloses: %d total %d failed",
		   atomic_read(&sent[SMB2_CLOSE_HE]),
		   atomic_read(&failed[SMB2_CLOSE_HE]));
	seq_printf(m, "\nFlushes: %d total %d failed",
		   atomic_read(&sent[SMB2_FLUSH_HE]),
		   atomic_read(&failed[SMB2_FLUSH_HE]));
	seq_printf(m, "\nReads: %d total %d failed",
		   atomic_read(&sent[SMB2_READ_HE]),
		   atomic_read(&failed[SMB2_READ_HE]));
	seq_printf(m, "\nWrites: %d total %d failed",
		   atomic_read(&sent[SMB2_WRITE_HE]),
		   atomic_read(&failed[SMB2_WRITE_HE]));
	seq_printf(m, "\nLocks: %d total %d failed",
		   atomic_read(&sent[SMB2_LOCK_HE]),
		   atomic_read(&failed[SMB2_LOCK_HE]));
	seq_printf(m, "\nIOCTLs: %d total %d failed",
		   atomic_read(&sent[SMB2_IOCTL_HE]),
		   atomic_read(&failed[SMB2_IOCTL_HE]));
	seq_printf(m, "\nQueryDirectories: %d total %d failed",
		   atomic_read(&sent[SMB2_QUERY_DIRECTORY_HE]),
		   atomic_read(&failed[SMB2_QUERY_DIRECTORY_HE]));
	seq_printf(m, "\nChangeNotifies: %d total %d failed",
		   atomic_read(&sent[SMB2_CHANGE_NOTIFY_HE]),
		   atomic_read(&failed[SMB2_CHANGE_NOTIFY_HE]));
	seq_printf(m, "\nQueryInfos: %d total %d failed",
		   atomic_read(&sent[SMB2_QUERY_INFO_HE]),
		   atomic_read(&failed[SMB2_QUERY_INFO_HE]));
	seq_printf(m, "\nSetInfos: %d total %d failed",
		   atomic_read(&sent[SMB2_SET_INFO_HE]),
		   atomic_read(&failed[SMB2_SET_INFO_HE]));
	seq_printf(m, "\nOplockBreaks: %d sent %d failed",
		   atomic_read(&sent[SMB2_OPLOCK_BREAK_HE]),
		   atomic_read(&failed[SMB2_OPLOCK_BREAK_HE]));
}