smb2_clear_stats(struct cifs_tcon *tcon)
{
	int i;

	for (i = 0; i < NUMBER_OF_SMB2_COMMANDS; i++) {
		atomic_set(&tcon->stats.smb2_stats.smb2_com_sent[i], 0);
		atomic_set(&tcon->stats.smb2_stats.smb2_com_failed[i], 0);
	}
}