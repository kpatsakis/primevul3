nodelist_add_node_and_family(smartlist_t *sl, const node_t *node)
{
  const smartlist_t *all_nodes = nodelist_get_list();
  const smartlist_t *declared_family;
  const or_options_t *options = get_options();

  tor_assert(node);

  declared_family = node_get_declared_family(node);

  /* Let's make sure that we have the node itself, if it's a real node. */
  {
    const node_t *real_node = node_get_by_id(node->identity);
    if (real_node)
      smartlist_add(sl, (node_t*)real_node);
  }

  /* First, add any nodes with similar network addresses. */
  if (options->EnforceDistinctSubnets) {
    tor_addr_t node_addr;
    node_get_addr(node, &node_addr);

    SMARTLIST_FOREACH_BEGIN(all_nodes, const node_t *, node2) {
      tor_addr_t a;
      node_get_addr(node2, &a);
      if (addrs_in_same_network_family(&a, &node_addr))
        smartlist_add(sl, (void*)node2);
    } SMARTLIST_FOREACH_END(node2);
  }

  /* Now, add all nodes in the declared_family of this node, if they
   * also declare this node to be in their family. */
  if (declared_family) {
    /* Add every r such that router declares familyness with node, and node
     * declares familyhood with router. */
    SMARTLIST_FOREACH_BEGIN(declared_family, const char *, name) {
      const node_t *node2;
      const smartlist_t *family2;
      if (!(node2 = node_get_by_nickname(name, 0)))
        continue;
      if (!(family2 = node_get_declared_family(node2)))
        continue;
      SMARTLIST_FOREACH_BEGIN(family2, const char *, name2) {
          if (node_nickname_matches(node, name2)) {
            smartlist_add(sl, (void*)node2);
            break;
          }
      } SMARTLIST_FOREACH_END(name2);
    } SMARTLIST_FOREACH_END(name);
  }

  /* If the user declared any families locally, honor those too. */
  if (options->NodeFamilySets) {
    SMARTLIST_FOREACH(options->NodeFamilySets, const routerset_t *, rs, {
      if (routerset_contains_node(rs, node)) {
        routerset_get_all_nodes(sl, rs, NULL, 0);
      }
    });
  }
}
