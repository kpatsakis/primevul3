node_has_descriptor(const node_t *node)
{
  return (node->ri ||
          (node->rs && node->md));
}
