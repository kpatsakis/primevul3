num_bridges_usable(void)
{
  int n_options = 0;

  tor_assert(get_options()->UseBridges);
  guard_selection_t *gs  = get_guard_selection_info();
  tor_assert(gs->type == GS_TYPE_BRIDGE);

  SMARTLIST_FOREACH_BEGIN(gs->sampled_entry_guards, entry_guard_t *, guard) {
    if (guard->is_reachable == GUARD_REACHABLE_NO)
      continue;
    if (tor_digest_is_zero(guard->identity))
      continue;
    const node_t *node = node_get_by_id(guard->identity);
    if (node && node->ri)
      ++n_options;
  } SMARTLIST_FOREACH_END(guard);

  return n_options;
}
