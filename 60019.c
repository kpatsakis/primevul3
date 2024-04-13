select_entry_guard_for_circuit(guard_selection_t *gs,
                               guard_usage_t usage,
                               const entry_guard_restriction_t *rst,
                               unsigned *state_out)
{
  const int need_descriptor = (usage == GUARD_USAGE_TRAFFIC);
  tor_assert(gs);
  tor_assert(state_out);

  if (!gs->primary_guards_up_to_date)
    entry_guards_update_primary(gs);

  int num_entry_guards = get_n_primary_guards_to_use(usage);
  smartlist_t *usable_primary_guards = smartlist_new();

  /* "If any entry in PRIMARY_GUARDS has {is_reachable} status of
      <maybe> or <yes>, return the first such guard." */
  SMARTLIST_FOREACH_BEGIN(gs->primary_entry_guards, entry_guard_t *, guard) {
    entry_guard_consider_retry(guard);
    if (! entry_guard_obeys_restriction(guard, rst))
      continue;
    if (guard->is_reachable != GUARD_REACHABLE_NO) {
      if (need_descriptor && !guard_has_descriptor(guard)) {
        continue;
      }
      *state_out = GUARD_CIRC_STATE_USABLE_ON_COMPLETION;
      guard->last_tried_to_connect = approx_time();
      smartlist_add(usable_primary_guards, guard);
      if (smartlist_len(usable_primary_guards) >= num_entry_guards)
        break;
    }
  } SMARTLIST_FOREACH_END(guard);

  if (smartlist_len(usable_primary_guards)) {
    entry_guard_t *guard = smartlist_choose(usable_primary_guards);
    smartlist_free(usable_primary_guards);
    log_info(LD_GUARD, "Selected primary guard %s for circuit.",
             entry_guard_describe(guard));
    return guard;
  }
  smartlist_free(usable_primary_guards);

  /* "Otherwise, if the ordered intersection of {CONFIRMED_GUARDS}
      and {USABLE_FILTERED_GUARDS} is nonempty, return the first
      entry in that intersection that has {is_pending} set to
      false." */
  SMARTLIST_FOREACH_BEGIN(gs->confirmed_entry_guards, entry_guard_t *, guard) {
    if (guard->is_primary)
      continue; /* we already considered this one. */
    if (! entry_guard_obeys_restriction(guard, rst))
      continue;
    entry_guard_consider_retry(guard);
    if (guard->is_usable_filtered_guard && ! guard->is_pending) {
      if (need_descriptor && !guard_has_descriptor(guard))
        continue; /* not a bug */
      guard->is_pending = 1;
      guard->last_tried_to_connect = approx_time();
      *state_out = GUARD_CIRC_STATE_USABLE_IF_NO_BETTER_GUARD;
      log_info(LD_GUARD, "No primary guards available. Selected confirmed "
               "guard %s for circuit. Will try other guards before using "
               "this circuit.",
               entry_guard_describe(guard));
      return guard;
    }
  } SMARTLIST_FOREACH_END(guard);

  /* "Otherwise, if there is no such entry, select a member at
      random from {USABLE_FILTERED_GUARDS}." */
  {
    entry_guard_t *guard;
    unsigned flags = 0;
    if (need_descriptor)
      flags |= SAMPLE_EXCLUDE_NO_DESCRIPTOR;
    guard = sample_reachable_filtered_entry_guards(gs,
                                                   rst,
                                                   SAMPLE_EXCLUDE_CONFIRMED |
                                                   SAMPLE_EXCLUDE_PRIMARY |
                                                   SAMPLE_EXCLUDE_PENDING |
                                                   flags);
    if (guard == NULL) {
      log_info(LD_GUARD, "Absolutely no sampled guards were available. "
               "Marking all guards for retry and starting from top again.");
      mark_all_guards_maybe_reachable(gs);
      return NULL;
    }
    guard->is_pending = 1;
    guard->last_tried_to_connect = approx_time();
    *state_out = GUARD_CIRC_STATE_USABLE_IF_NO_BETTER_GUARD;
    log_info(LD_GUARD, "No primary or confirmed guards available. Selected "
             "random guard %s for circuit. Will try other guards before "
             "using this circuit.",
             entry_guard_describe(guard));
    return guard;
  }
}
