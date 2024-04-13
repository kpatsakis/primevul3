remove_all_entry_guards_for_guard_selection(guard_selection_t *gs)
{
  tor_assert(gs != NULL);
  char *old_name = tor_strdup(gs->name);
  guard_selection_type_t old_type = gs->type;

  SMARTLIST_FOREACH(gs->sampled_entry_guards, entry_guard_t *, entry, {
    control_event_guard(entry->nickname, entry->identity, "DROPPED");
  });

  if (gs == curr_guard_context) {
    curr_guard_context = NULL;
  }

  smartlist_remove(guard_contexts, gs);
  guard_selection_free(gs);

  gs = get_guard_selection_by_name(old_name, old_type, 1);
  entry_guards_changed_for_guard_selection(gs);
  tor_free(old_name);
}
