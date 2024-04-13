nodelist_remove_routerinfo(routerinfo_t *ri)
{
  node_t *node = node_get_mutable_by_id(ri->cache_info.identity_digest);
  if (node && node->ri == ri) {
    node->ri = NULL;
    if (! node_is_usable(node)) {
      nodelist_drop_node(node, 1);
      node_free(node);
    }
  }
}
