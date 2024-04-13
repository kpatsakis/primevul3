node_set_country(node_t *node)
{
  tor_addr_t addr = TOR_ADDR_NULL;

  /* XXXXipv6 */
  if (node->rs)
    tor_addr_from_ipv4h(&addr, node->rs->addr);
  else if (node->ri)
    tor_addr_from_ipv4h(&addr, node->ri->addr);

  node->country = geoip_get_country_by_addr(&addr);
}
