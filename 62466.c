static int commit_super_block(const struct exfat* ef)
{
	if (exfat_pwrite(ef->dev, ef->sb, sizeof(struct exfat_super_block), 0) < 0)
	{
		exfat_error("failed to write super block");
		return 1;
	}
	return exfat_fsync(ef->dev);
}
