nodelist_set_consensus(networkstatus_t *ns)
{
  const or_options_t *options = get_options();
  int authdir = authdir_mode_v3(options);

  init_nodelist();
  if (ns->flavor == FLAV_MICRODESC)
    (void) get_microdesc_cache(); /* Make sure it exists first. */

  SMARTLIST_FOREACH(the_nodelist->nodes, node_t *, node,
                    node->rs = NULL);

  SMARTLIST_FOREACH_BEGIN(ns->routerstatus_list, routerstatus_t *, rs) {
    node_t *node = node_get_or_create(rs->identity_digest);
    node->rs = rs;
    if (ns->flavor == FLAV_MICRODESC) {
      if (node->md == NULL ||
          tor_memneq(node->md->digest,rs->descriptor_digest,DIGEST256_LEN)) {
        if (node->md)
          node->md->held_by_nodes--;
        node->md = microdesc_cache_lookup_by_digest256(NULL,
                                                       rs->descriptor_digest);
        if (node->md)
          node->md->held_by_nodes++;
      }
    }

    node_set_country(node);

    /* If we're not an authdir, believe others. */
    if (!authdir) {
      node->is_valid = rs->is_valid;
      node->is_running = rs->is_flagged_running;
      node->is_fast = rs->is_fast;
      node->is_stable = rs->is_stable;
      node->is_possible_guard = rs->is_possible_guard;
      node->is_exit = rs->is_exit;
      node->is_bad_exit = rs->is_bad_exit;
      node->is_hs_dir = rs->is_hs_dir;
      node->ipv6_preferred = 0;
      if (fascist_firewall_prefer_ipv6_orport(options) &&
          (tor_addr_is_null(&rs->ipv6_addr) == 0 ||
           (node->md && tor_addr_is_null(&node->md->ipv6_addr) == 0)))
        node->ipv6_preferred = 1;
    }

  } SMARTLIST_FOREACH_END(rs);

  nodelist_purge();

  if (! authdir) {
    SMARTLIST_FOREACH_BEGIN(the_nodelist->nodes, node_t *, node) {
      /* We have no routerstatus for this router. Clear flags so we can skip
       * it, maybe.*/
      if (!node->rs) {
        tor_assert(node->ri); /* if it had only an md, or nothing, purge
                               * would have removed it. */
        if (node->ri->purpose == ROUTER_PURPOSE_GENERAL) {
          /* Clear all flags. */
          node->is_valid = node->is_running = node->is_hs_dir =
            node->is_fast = node->is_stable =
            node->is_possible_guard = node->is_exit =
            node->is_bad_exit = node->ipv6_preferred = 0;
        }
      }
    } SMARTLIST_FOREACH_END(node);
  }
}
