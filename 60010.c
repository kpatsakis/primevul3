num_reachable_filtered_guards(guard_selection_t *gs,
                              const entry_guard_restriction_t *rst)
{
  int n_reachable_filtered_guards = 0;
  SMARTLIST_FOREACH_BEGIN(gs->sampled_entry_guards, entry_guard_t *, guard) {
    entry_guard_consider_retry(guard);
    if (! entry_guard_obeys_restriction(guard, rst))
      continue;
    if (guard->is_usable_filtered_guard)
      ++n_reachable_filtered_guards;
  } SMARTLIST_FOREACH_END(guard);
  return n_reachable_filtered_guards;
}
