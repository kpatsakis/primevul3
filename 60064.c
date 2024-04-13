node_id_hash(const node_t *node)
{
  return (unsigned) siphash24g(node->identity, DIGEST_LEN);
}
