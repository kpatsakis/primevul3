std::vector<std::string> Extension::GetDistinctHostsForDisplay(
    const URLPatternList& list) {
  return GetDistinctHosts(list, true);
}
