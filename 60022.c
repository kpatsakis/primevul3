MOCK_IMPL(const node_t *,
node_get_by_id,(const char *identity_digest))
{
  return node_get_mutable_by_id(identity_digest);
}
