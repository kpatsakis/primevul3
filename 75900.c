bool Extension::IsElevatedHostList(
    const URLPatternList& old_list, const URLPatternList& new_list) {

  std::vector<std::string> new_hosts = GetDistinctHosts(new_list, false);
  std::vector<std::string> old_hosts = GetDistinctHosts(old_list, false);

  std::set<std::string> old_hosts_set(old_hosts.begin(), old_hosts.end());
  std::set<std::string> new_hosts_set(new_hosts.begin(), new_hosts.end());
  std::set<std::string> new_hosts_only;

  std::set_difference(new_hosts_set.begin(), new_hosts_set.end(),
                      old_hosts_set.begin(), old_hosts_set.end(),
                      std::inserter(new_hosts_only, new_hosts_only.begin()));

  return !new_hosts_only.empty();
}
