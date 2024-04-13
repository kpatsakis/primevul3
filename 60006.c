mark_primary_guards_maybe_reachable(guard_selection_t *gs)
{
  tor_assert(gs);

  if (!gs->primary_guards_up_to_date)
    entry_guards_update_primary(gs);

  SMARTLIST_FOREACH_BEGIN(gs->primary_entry_guards, entry_guard_t *, guard) {
    mark_guard_maybe_reachable(guard);
  } SMARTLIST_FOREACH_END(guard);
}
