node_get_addr(const node_t *node, tor_addr_t *addr_out)
{
  tor_addr_port_t ap;
  node_get_prim_orport(node, &ap);
  tor_addr_copy(addr_out, &ap.addr);
}
