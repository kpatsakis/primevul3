select_and_add_guard_item_for_sample(guard_selection_t *gs,
                                     smartlist_t *eligible_guards)
{
  entry_guard_t *added_guard;
  if (gs->type == GS_TYPE_BRIDGE) {
    const bridge_info_t *bridge = smartlist_choose(eligible_guards);
    if (BUG(!bridge))
      return NULL; // LCOV_EXCL_LINE
    smartlist_remove(eligible_guards, bridge);
    added_guard = entry_guard_add_bridge_to_sample(gs, bridge);
  } else {
    const node_t *node =
      node_sl_choose_by_bandwidth(eligible_guards, WEIGHT_FOR_GUARD);
    if (BUG(!node))
      return NULL; // LCOV_EXCL_LINE
    smartlist_remove(eligible_guards, node);
    added_guard = entry_guard_add_to_sample(gs, node);
  }

  return added_guard;
}
