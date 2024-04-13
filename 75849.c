void NavigationController::CreateNavigationEntriesFromTabNavigations(
    const std::vector<TabNavigation>& navigations,
    std::vector<linked_ptr<NavigationEntry> >* entries) {
  int page_id = 0;
  for (std::vector<TabNavigation>::const_iterator i =
           navigations.begin(); i != navigations.end(); ++i, ++page_id) {
    linked_ptr<NavigationEntry> entry(i->ToNavigationEntry(page_id, profile_));
    entries->push_back(entry);
  }
}
