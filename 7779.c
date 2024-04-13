ensure_pattern_sub_nodes(node_t *node,
                         apr_pool_t *result_pool)
{
  if (node->pattern_sub_nodes == NULL)
    node->pattern_sub_nodes = apr_pcalloc(result_pool,
                                          sizeof(*node->pattern_sub_nodes));

  return node->pattern_sub_nodes;
}