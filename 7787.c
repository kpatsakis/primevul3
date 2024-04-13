ensure_node(node_t **node,
            authz_rule_segment_t *segment,
            apr_pool_t *result_pool)
{
  if (!*node)
    *node = create_node(segment, result_pool);

  return *node;
}