mark_all_guards_maybe_reachable(guard_selection_t *gs)
{
  tor_assert(gs);

  SMARTLIST_FOREACH_BEGIN(gs->sampled_entry_guards, entry_guard_t *, guard) {
    mark_guard_maybe_reachable(guard);
  } SMARTLIST_FOREACH_END(guard);
}
