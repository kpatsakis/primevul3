nodelist_drop_node(node_t *node, int remove_from_ht)
{
  node_t *tmp;
  int idx;
  if (remove_from_ht) {
    tmp = HT_REMOVE(nodelist_map, &the_nodelist->nodes_by_id, node);
    tor_assert(tmp == node);
  }

  idx = node->nodelist_idx;
  tor_assert(idx >= 0);

  tor_assert(node == smartlist_get(the_nodelist->nodes, idx));
  smartlist_del(the_nodelist->nodes, idx);
  if (idx < smartlist_len(the_nodelist->nodes)) {
    tmp = smartlist_get(the_nodelist->nodes, idx);
    tmp->nodelist_idx = idx;
  }
  node->nodelist_idx = -1;
}
