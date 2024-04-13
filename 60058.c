node_has_curve25519_onion_key(const node_t *node)
{
  if (!node)
    return 0;

  if (node->ri)
    return routerinfo_has_curve25519_onion_key(node->ri);
  else if (node->md)
    return microdesc_has_curve25519_onion_key(node->md);
  else
    return 0;
}
