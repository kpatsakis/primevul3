update_guard_selection_choice(const or_options_t *options)
{
  if (!curr_guard_context) {
    create_initial_guard_context();
    return 1;
  }

  guard_selection_type_t type = GS_TYPE_INFER;
  const char *new_name = choose_guard_selection(
                             options,
                             networkstatus_get_live_consensus(approx_time()),
                             curr_guard_context,
                             &type);
  tor_assert(new_name);
  tor_assert(type != GS_TYPE_INFER);

  const char *cur_name = curr_guard_context->name;
  if (! strcmp(cur_name, new_name)) {
    log_debug(LD_GUARD,
              "Staying with guard context \"%s\" (no change)", new_name);
    return 0; // No change
  }

  log_notice(LD_GUARD, "Switching to guard context \"%s\" (was using \"%s\")",
             new_name, cur_name);
  guard_selection_t *new_guard_context;
  new_guard_context = get_guard_selection_by_name(new_name, type, 1);
  tor_assert(new_guard_context);
  tor_assert(new_guard_context != curr_guard_context);
  curr_guard_context = new_guard_context;

  return 1;
}
