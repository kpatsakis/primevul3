node_supports_ed25519_link_authentication(const node_t *node)
{
  /* XXXX Oh hm. What if some day in the future there are link handshake
   * versions that aren't 3 but which are ed25519 */
  if (! node_get_ed25519_id(node))
    return 0;
  if (node->ri) {
    const char *protos = node->ri->protocol_list;
    if (protos == NULL)
      return 0;
    return protocol_list_supports_protocol(protos, PRT_LINKAUTH, 3);
  }
  if (node->rs) {
    return node->rs->supports_ed25519_link_handshake;
  }
  tor_assert_nonfatal_unreached_once();
  return 0;
}
