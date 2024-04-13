nodes_in_same_family(const node_t *node1, const node_t *node2)
{
  const or_options_t *options = get_options();

  /* Are they in the same family because of their addresses? */
  if (options->EnforceDistinctSubnets) {
    tor_addr_t a1, a2;
    node_get_addr(node1, &a1);
    node_get_addr(node2, &a2);
    if (addrs_in_same_network_family(&a1, &a2))
      return 1;
  }

  /* Are they in the same family because the agree they are? */
  {
    const smartlist_t *f1, *f2;
    f1 = node_get_declared_family(node1);
    f2 = node_get_declared_family(node2);
    if (f1 && f2 &&
        node_in_nickname_smartlist(f1, node2) &&
        node_in_nickname_smartlist(f2, node1))
      return 1;
  }

  /* Are they in the same option because the user says they are? */
  if (options->NodeFamilySets) {
    SMARTLIST_FOREACH(options->NodeFamilySets, const routerset_t *, rs, {
        if (routerset_contains_node(rs, node1) &&
            routerset_contains_node(rs, node2))
          return 1;
      });
  }

  return 0;
}
