void __init vfs_caches_init_early(void)
{
	dcache_init_early();
	inode_init_early();
}
