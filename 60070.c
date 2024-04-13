node_is_named(const node_t *node)
{
  const char *named_id;
  const char *nickname = node_get_nickname(node);
  if (!nickname)
    return 0;
  named_id = networkstatus_get_router_digest_by_nickname(nickname);
  if (!named_id)
    return 0;
  return tor_memeq(named_id, node->identity, DIGEST_LEN);
}
