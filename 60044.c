node_get_ed25519_id(const node_t *node)
{
  if (node->ri) {
    if (node->ri->cache_info.signing_key_cert) {
      const ed25519_public_key_t *pk =
        &node->ri->cache_info.signing_key_cert->signing_key;
      if (BUG(ed25519_public_key_is_zero(pk)))
        goto try_the_md;
      return pk;
    }
  }
 try_the_md:
  if (node->md) {
    if (node->md->ed25519_identity_pkey) {
      return node->md->ed25519_identity_pkey;
    }
  }
  return NULL;
}
