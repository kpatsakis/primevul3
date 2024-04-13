node_get_published_on(const node_t *node)
{
  if (node->ri)
    return node->ri->cache_info.published_on;
  else
    return 0;
}
