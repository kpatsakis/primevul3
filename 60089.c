router_find_exact_exit_enclave(const char *address, uint16_t port)
{/*XXXX MOVE*/
  uint32_t addr;
  struct in_addr in;
  tor_addr_t a;
  const or_options_t *options = get_options();

  if (!tor_inet_aton(address, &in))
    return NULL; /* it's not an IP already */
  addr = ntohl(in.s_addr);

  tor_addr_from_ipv4h(&a, addr);

  SMARTLIST_FOREACH(nodelist_get_list(), const node_t *, node, {
    if (node_get_addr_ipv4h(node) == addr &&
        node->is_running &&
        compare_tor_addr_to_node_policy(&a, port, node) ==
          ADDR_POLICY_ACCEPTED &&
        !routerset_contains_node(options->ExcludeExitNodesUnion_, node))
      return node;
  });
  return NULL;
}
