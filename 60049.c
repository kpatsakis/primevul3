node_get_pref_dirport(const node_t *node, tor_addr_port_t *ap_out)
{
  tor_assert(ap_out);

  if (node_ipv6_dir_preferred(node)) {
    node_get_pref_ipv6_dirport(node, ap_out);
  } else {
    /* the primary DirPort is always on IPv4 */
    node_get_prim_dirport(node, ap_out);
  }
}
