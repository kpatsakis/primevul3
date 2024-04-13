node_is_usable(const node_t *node)
{
  return (node->rs) || (node->ri);
}
