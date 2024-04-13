node_is_me(const node_t *node)
{
  return router_digest_is_me(node->identity);
}
