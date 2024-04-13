node_get_rsa_id_digest(const node_t *node)
{
  tor_assert(node);
  return (const uint8_t*)node->identity;
}
