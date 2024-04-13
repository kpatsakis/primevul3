node_get_prim_orport(const node_t *node, tor_addr_port_t *ap_out)
{
  node_assert_ok(node);
  tor_assert(ap_out);

  /* Check ri first, because rewrite_node_address_for_bridge() updates
   * node->ri with the configured bridge address. */

  RETURN_IPV4_AP(node->ri, or_port, ap_out);
  RETURN_IPV4_AP(node->rs, or_port, ap_out);
  /* Microdescriptors only have an IPv6 address */

  return -1;
}
