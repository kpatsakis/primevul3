static uint64_t rootdir_size(const struct exfat* ef)
{
	uint64_t clusters = 0;
	cluster_t rootdir_cluster = le32_to_cpu(ef->sb->rootdir_cluster);

	while (!CLUSTER_INVALID(rootdir_cluster))
	{
		clusters++;
		/* root directory cannot be contiguous because there is no flag
		   to indicate this */
		rootdir_cluster = exfat_next_cluster(ef, ef->root, rootdir_cluster);
	}
	if (rootdir_cluster != EXFAT_CLUSTER_END)
	{
		exfat_error("bad cluster %#x while reading root directory",
				rootdir_cluster);
		return 0;
	}
	return clusters * CLUSTER_SIZE(*ef->sb);
}
