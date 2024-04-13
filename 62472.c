static int prepare_super_block(const struct exfat* ef)
{
	if (le16_to_cpu(ef->sb->volume_state) & EXFAT_STATE_MOUNTED)
		exfat_warn("volume was not unmounted cleanly");

	if (ef->ro)
		return 0;

	ef->sb->volume_state = cpu_to_le16(
			le16_to_cpu(ef->sb->volume_state) | EXFAT_STATE_MOUNTED);
	return commit_super_block(ef);
}
