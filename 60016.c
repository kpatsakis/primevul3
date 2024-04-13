sample_reachable_filtered_entry_guards(guard_selection_t *gs,
                                       const entry_guard_restriction_t *rst,
                                       unsigned flags)
{
  tor_assert(gs);
  entry_guard_t *result = NULL;
  const unsigned exclude_confirmed = flags & SAMPLE_EXCLUDE_CONFIRMED;
  const unsigned exclude_primary = flags & SAMPLE_EXCLUDE_PRIMARY;
  const unsigned exclude_pending = flags & SAMPLE_EXCLUDE_PENDING;
  const unsigned no_update_primary = flags & SAMPLE_NO_UPDATE_PRIMARY;
  const unsigned need_descriptor = flags & SAMPLE_EXCLUDE_NO_DESCRIPTOR;

  SMARTLIST_FOREACH_BEGIN(gs->sampled_entry_guards, entry_guard_t *, guard) {
    entry_guard_consider_retry(guard);
  } SMARTLIST_FOREACH_END(guard);

  const int n_reachable_filtered = num_reachable_filtered_guards(gs, rst);

  log_info(LD_GUARD, "Trying to sample a reachable guard: We know of %d "
           "in the USABLE_FILTERED set.", n_reachable_filtered);

  const int min_filtered_sample = get_min_filtered_sample_size();
  if (n_reachable_filtered < min_filtered_sample) {
    log_info(LD_GUARD, "  (That isn't enough. Trying to expand the sample.)");
    entry_guards_expand_sample(gs);
  }

  if (exclude_primary && !gs->primary_guards_up_to_date && !no_update_primary)
    entry_guards_update_primary(gs);

  /* Build the set of reachable filtered guards. */
  smartlist_t *reachable_filtered_sample = smartlist_new();
  SMARTLIST_FOREACH_BEGIN(gs->sampled_entry_guards, entry_guard_t *, guard) {
    entry_guard_consider_retry(guard);// redundant, but cheap.
    if (! entry_guard_obeys_restriction(guard, rst))
      continue;
    if (! guard->is_usable_filtered_guard)
      continue;
    if (exclude_confirmed && guard->confirmed_idx >= 0)
      continue;
    if (exclude_primary && guard->is_primary)
      continue;
    if (exclude_pending && guard->is_pending)
      continue;
    if (need_descriptor && !guard_has_descriptor(guard))
      continue;
    smartlist_add(reachable_filtered_sample, guard);
  } SMARTLIST_FOREACH_END(guard);

  log_info(LD_GUARD, "  (After filters [%x], we have %d guards to consider.)",
           flags, smartlist_len(reachable_filtered_sample));

  if (smartlist_len(reachable_filtered_sample)) {
    result = smartlist_choose(reachable_filtered_sample);
    log_info(LD_GUARD, "  (Selected %s.)",
             result ? entry_guard_describe(result) : "<null>");
  }
  smartlist_free(reachable_filtered_sample);

  return result;
}
