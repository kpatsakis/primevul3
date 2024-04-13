void ConfigureEntriesForRestore(
    std::vector<linked_ptr<NavigationEntry> >* entries,
    bool from_last_session) {
  for (size_t i = 0; i < entries->size(); ++i) {
    (*entries)[i]->set_transition_type(PageTransition::RELOAD);
    (*entries)[i]->set_restore_type(from_last_session ?
        NavigationEntry::RESTORE_LAST_SESSION :
        NavigationEntry::RESTORE_CURRENT_SESSION);
    SetContentStateIfEmpty((*entries)[i].get());
  }
}
