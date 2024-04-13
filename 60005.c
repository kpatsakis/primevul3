mark_guard_maybe_reachable(entry_guard_t *guard)
{
  if (guard->is_reachable != GUARD_REACHABLE_NO) {
    return;
  }

  /* Note that we do not clear failing_since: this guard is now only
   * _maybe-reachable_. */
  guard->is_reachable = GUARD_REACHABLE_MAYBE;
  if (guard->is_filtered_guard)
    guard->is_usable_filtered_guard = 1;
}
