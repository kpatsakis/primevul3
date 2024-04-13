void NavigationController::RestoreFromState(
    const std::vector<TabNavigation>& navigations,
    int selected_navigation,
    bool from_last_session) {
  DCHECK(entry_count() == 0 && !pending_entry());
  DCHECK(selected_navigation >= 0 &&
         selected_navigation < static_cast<int>(navigations.size()));

  needs_reload_ = true;
  CreateNavigationEntriesFromTabNavigations(navigations, &entries_);

  FinishRestore(selected_navigation, from_last_session);
}
