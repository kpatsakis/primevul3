guards_retry_optimistic(const or_options_t *options)
{
  if (! entry_list_is_constrained(options))
    return 0;

  mark_primary_guards_maybe_reachable(get_guard_selection_info());

  return 1;
}
