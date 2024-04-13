node_get_pref_ipv6_orport(const node_t *node, tor_addr_port_t *ap_out)
{
  node_assert_ok(node);
  tor_assert(ap_out);

  /* Check ri first, because rewrite_node_address_for_bridge() updates
   * node->ri with the configured bridge address.
   * Prefer rs over md for consistency with the fascist_firewall_* functions.
   * Check if the address or port are valid, and try another alternative
   * if they are not. */

  if (node->ri && tor_addr_port_is_valid(&node->ri->ipv6_addr,
                                         node->ri->ipv6_orport, 0)) {
    tor_addr_copy(&ap_out->addr, &node->ri->ipv6_addr);
    ap_out->port = node->ri->ipv6_orport;
  } else if (node->rs && tor_addr_port_is_valid(&node->rs->ipv6_addr,
                                                 node->rs->ipv6_orport, 0)) {
    tor_addr_copy(&ap_out->addr, &node->rs->ipv6_addr);
    ap_out->port = node->rs->ipv6_orport;
  } else if (node->md && tor_addr_port_is_valid(&node->md->ipv6_addr,
                                                 node->md->ipv6_orport, 0)) {
    tor_addr_copy(&ap_out->addr, &node->md->ipv6_addr);
    ap_out->port = node->md->ipv6_orport;
  } else {
    tor_addr_make_null(&ap_out->addr, AF_INET6);
    ap_out->port = 0;
  }
}
