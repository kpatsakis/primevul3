node_has_ipv6_orport(const node_t *node)
{
  tor_addr_port_t ipv6_orport;
  node_get_pref_ipv6_orport(node, &ipv6_orport);
  return tor_addr_port_is_valid_ap(&ipv6_orport, 0);
}
