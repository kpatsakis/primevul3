node_addrs_changed(node_t *node)
{
  node->last_reachable = node->last_reachable6 = 0;
  node->country = -1;
}
