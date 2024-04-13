node_nickname_matches(const node_t *node, const char *nickname)
{
  const char *n = node_get_nickname(node);
  if (n && nickname[0]!='$' && !strcasecmp(n, nickname))
    return 1;
  return hex_digest_nickname_matches(nickname,
                                     node->identity,
                                     n,
                                     node_is_named(node));
}
