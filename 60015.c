remove_guard_from_confirmed_and_primary_lists(guard_selection_t *gs,
                                              entry_guard_t *guard)
{
  if (guard->is_primary) {
    guard->is_primary = 0;
    smartlist_remove_keeporder(gs->primary_entry_guards, guard);
  } else {
    if (BUG(smartlist_contains(gs->primary_entry_guards, guard))) {
      smartlist_remove_keeporder(gs->primary_entry_guards, guard);
    }
  }

  if (guard->confirmed_idx >= 0) {
    smartlist_remove_keeporder(gs->confirmed_entry_guards, guard);
    guard->confirmed_idx = -1;
    guard->confirmed_on_date = 0;
  } else {
    if (BUG(smartlist_contains(gs->confirmed_entry_guards, guard))) {
      smartlist_remove_keeporder(gs->confirmed_entry_guards, guard);
    }
  }
}
