node_ipv6_dir_preferred(const node_t *node)
{
  const or_options_t *options = get_options();
  tor_addr_port_t ipv4_addr;
  node_assert_ok(node);

  /* node->ipv6_preferred is set from fascist_firewall_prefer_ipv6_orport(),
   * so we can't use it to determine DirPort IPv6 preference.
   * This means that bridge clients will use IPv4 DirPorts by default.
   */
  if (!fascist_firewall_use_ipv6(options)) {
    return 0;
  } else if (node_get_prim_dirport(node, &ipv4_addr)
      || fascist_firewall_prefer_ipv6_dirport(get_options())) {
    return node_has_ipv6_dirport(node);
  }
  return 0;
}
