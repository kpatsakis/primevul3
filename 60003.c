make_guard_confirmed(guard_selection_t *gs, entry_guard_t *guard)
{
  if (BUG(guard->confirmed_on_date && guard->confirmed_idx >= 0))
    return; // LCOV_EXCL_LINE

  if (BUG(smartlist_contains(gs->confirmed_entry_guards, guard)))
    return; // LCOV_EXCL_LINE

  const int GUARD_LIFETIME = get_guard_lifetime();
  guard->confirmed_on_date = randomize_time(approx_time(), GUARD_LIFETIME/10);

  log_info(LD_GUARD, "Marking %s as a confirmed guard (index %d)",
           entry_guard_describe(guard),
           gs->next_confirmed_idx);

  guard->confirmed_idx = gs->next_confirmed_idx++;
  smartlist_add(gs->confirmed_entry_guards, guard);

  gs->primary_guards_up_to_date = 0;

  entry_guards_changed_for_guard_selection(gs);
}
