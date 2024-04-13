node_has_ipv6_dirport(const node_t *node)
{
  tor_addr_port_t ipv6_dirport;
  node_get_pref_ipv6_dirport(node, &ipv6_dirport);
  return tor_addr_port_is_valid_ap(&ipv6_dirport, 0);
}
