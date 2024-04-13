node_get_all_orports(const node_t *node)
{
  smartlist_t *sl = smartlist_new();
  int valid = 0;

  /* Find a valid IPv4 address and port */
  if (node->ri != NULL) {
    SL_ADD_NEW_IPV4_AP(node->ri, or_port, sl, valid);
  }

  /* If we didn't find a valid address/port in the ri, try the rs */
  if (!valid && node->rs != NULL) {
    SL_ADD_NEW_IPV4_AP(node->rs, or_port, sl, valid);
  }

  /* Find a valid IPv6 address and port */
  valid = 0;
  if (node->ri != NULL) {
    SL_ADD_NEW_IPV6_AP(node->ri, ipv6_orport, sl, valid);
  }

  if (!valid && node->rs != NULL) {
    SL_ADD_NEW_IPV6_AP(node->rs, ipv6_orport, sl, valid);
  }

  if (!valid && node->md != NULL) {
    SL_ADD_NEW_IPV6_AP(node->md, ipv6_orport, sl, valid);
  }

  return sl;
}
