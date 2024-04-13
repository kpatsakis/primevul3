nodelist_set_routerinfo(routerinfo_t *ri, routerinfo_t **ri_old_out)
{
  node_t *node;
  const char *id_digest;
  int had_router = 0;
  tor_assert(ri);

  init_nodelist();
  id_digest = ri->cache_info.identity_digest;
  node = node_get_or_create(id_digest);

  if (node->ri) {
    if (!routers_have_same_or_addrs(node->ri, ri)) {
      node_addrs_changed(node);
    }
    had_router = 1;
    if (ri_old_out)
      *ri_old_out = node->ri;
  } else {
    if (ri_old_out)
      *ri_old_out = NULL;
  }
  node->ri = ri;

  if (node->country == -1)
    node_set_country(node);

  if (authdir_mode(get_options()) && !had_router) {
    const char *discard=NULL;
    uint32_t status = dirserv_router_get_status(ri, &discard, LOG_INFO);
    dirserv_set_node_flags_from_authoritative_status(node, status);
  }

  return node;
}
