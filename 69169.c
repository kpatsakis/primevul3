void destroy_extent_cache(void)
{
	kmem_cache_destroy(extent_node_slab);
	kmem_cache_destroy(extent_tree_slab);
}
