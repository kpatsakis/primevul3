node_in_nickname_smartlist(const smartlist_t *lst, const node_t *node)
{
  if (!lst) return 0;
  SMARTLIST_FOREACH(lst, const char *, name, {
    if (node_nickname_matches(node, name))
      return 1;
  });
  return 0;
}
