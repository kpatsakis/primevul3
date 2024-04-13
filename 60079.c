nodelist_find_nodes_with_microdesc(const microdesc_t *md)
{
  smartlist_t *result = smartlist_new();

  if (the_nodelist == NULL)
    return result;

  SMARTLIST_FOREACH_BEGIN(the_nodelist->nodes, node_t *, node) {
    if (node->md == md) {
      smartlist_add(result, node);
    }
  } SMARTLIST_FOREACH_END(node);

  return result;
}
