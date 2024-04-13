node_ed25519_id_matches(const node_t *node, const ed25519_public_key_t *id)
{
  const ed25519_public_key_t *node_id = node_get_ed25519_id(node);
  if (node_id == NULL || ed25519_public_key_is_zero(node_id)) {
    return id == NULL || ed25519_public_key_is_zero(id);
  } else {
    return id && ed25519_pubkey_eq(node_id, id);
  }
}
