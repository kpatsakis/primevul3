node_allows_single_hop_exits(const node_t *node)
{
  if (node && node->ri)
    return node->ri->allow_single_hop_exits;
  else
    return 0;
}
