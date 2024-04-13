node_get_pref_ipv6_dirport(const node_t *node, tor_addr_port_t *ap_out)
{
  node_assert_ok(node);
  tor_assert(ap_out);

  /* Check ri first, because rewrite_node_address_for_bridge() updates
   * node->ri with the configured bridge address.
   * Prefer rs over md for consistency with the fascist_firewall_* functions.
   * Check if the address or port are valid, and try another alternative
   * if they are not. */

  /* Assume IPv4 and IPv6 dirports are the same */
  if (node->ri && tor_addr_port_is_valid(&node->ri->ipv6_addr,
                                         node->ri->dir_port, 0)) {
    tor_addr_copy(&ap_out->addr, &node->ri->ipv6_addr);
    ap_out->port = node->ri->dir_port;
  } else if (node->rs && tor_addr_port_is_valid(&node->rs->ipv6_addr,
                                                node->rs->dir_port, 0)) {
    tor_addr_copy(&ap_out->addr, &node->rs->ipv6_addr);
    ap_out->port = node->rs->dir_port;
  } else {
    tor_addr_make_null(&ap_out->addr, AF_INET6);
    ap_out->port = 0;
  }
}
