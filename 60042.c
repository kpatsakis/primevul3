node_get_declared_family(const node_t *node)
{
  if (node->ri && node->ri->declared_family)
    return node->ri->declared_family;
  else if (node->md && node->md->family)
    return node->md->family;
  else
    return NULL;
}
