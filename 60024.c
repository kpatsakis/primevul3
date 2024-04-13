MOCK_IMPL(const node_t *,
node_get_by_nickname,(const char *nickname, int warn_if_unnamed))
{
  if (!the_nodelist)
    return NULL;

  /* Handle these cases: DIGEST, $DIGEST, $DIGEST=name, $DIGEST~name. */
  {
    const node_t *node;
    if ((node = node_get_by_hex_id(nickname)) != NULL)
      return node;
  }

  if (!strcasecmp(nickname, UNNAMED_ROUTER_NICKNAME))
    return NULL;

  /* Okay, so if we get here, the nickname is just a nickname.  Is there
   * a binding for it in the consensus? */
  {
    const char *named_id =
      networkstatus_get_router_digest_by_nickname(nickname);
    if (named_id)
      return node_get_by_id(named_id);
  }

  /* Is it marked as owned-by-someone-else? */
  if (networkstatus_nickname_is_unnamed(nickname)) {
    log_info(LD_GENERAL, "The name %s is listed as Unnamed: there is some "
             "router that holds it, but not one listed in the current "
             "consensus.", escaped(nickname));
    return NULL;
  }

  /* Okay, so the name is not canonical for anybody. */
  {
    smartlist_t *matches = smartlist_new();
    const node_t *choice = NULL;

    SMARTLIST_FOREACH_BEGIN(the_nodelist->nodes, node_t *, node) {
      if (!strcasecmp(node_get_nickname(node), nickname))
        smartlist_add(matches, node);
    } SMARTLIST_FOREACH_END(node);

    if (smartlist_len(matches)>1 && warn_if_unnamed) {
      int any_unwarned = 0;
      SMARTLIST_FOREACH_BEGIN(matches, node_t *, node) {
        if (!node->name_lookup_warned) {
          node->name_lookup_warned = 1;
          any_unwarned = 1;
        }
      } SMARTLIST_FOREACH_END(node);

      if (any_unwarned) {
        log_warn(LD_CONFIG, "There are multiple matches for the name %s, "
                 "but none is listed as Named in the directory consensus. "
                 "Choosing one arbitrarily.", nickname);
      }
    } else if (smartlist_len(matches)==1 && warn_if_unnamed) {
      char fp[HEX_DIGEST_LEN+1];
      node_t *node = smartlist_get(matches, 0);
      if (! node->name_lookup_warned) {
        base16_encode(fp, sizeof(fp), node->identity, DIGEST_LEN);
        log_warn(LD_CONFIG,
                 "You specified a server \"%s\" by name, but the directory "
                 "authorities do not have any key registered for this "
                 "nickname -- so it could be used by any server, not just "
                 "the one you meant. "
                 "To make sure you get the same server in the future, refer "
                 "to it by key, as \"$%s\".", nickname, fp);
        node->name_lookup_warned = 1;
      }
    }

    if (smartlist_len(matches))
      choice = smartlist_get(matches, 0);

    smartlist_free(matches);
    return choice;
  }
}
