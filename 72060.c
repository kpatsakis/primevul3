static void *alloc_request_simple(gfp_t gfp_mask, void *data)
{
	struct request_queue *q = data;

	return kmem_cache_alloc_node(request_cachep, gfp_mask, q->node);
}
