void NavigationController::FinishRestore(int selected_index,
                                         bool from_last_session) {
  DCHECK(selected_index >= 0 && selected_index < entry_count());
  ConfigureEntriesForRestore(&entries_, from_last_session);

  set_max_restored_page_id(static_cast<int32>(entry_count()));

  last_committed_entry_index_ = selected_index;
}
