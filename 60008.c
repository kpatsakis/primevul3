node_passes_guard_filter(const or_options_t *options,
                         const node_t *node)
{
  /* NOTE: Make sure that this function stays in sync with
   * options_transition_affects_entry_guards */
  if (routerset_contains_node(options->ExcludeNodes, node))
    return 0;

  if (options->EntryNodes &&
      !routerset_contains_node(options->EntryNodes, node))
    return 0;

  if (!fascist_firewall_allows_node(node, FIREWALL_OR_CONNECTION, 0))
    return 0;

  if (node_is_a_configured_bridge(node))
    return 0;

  return 1;
}
