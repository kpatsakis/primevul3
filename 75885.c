std::vector<std::string> Extension::GetDistinctHosts(
    const URLPatternList& host_patterns,
    bool include_rcd) {
  typedef std::vector<std::pair<std::string, std::string> > HostVector;
  HostVector hosts_best_rcd;
  for (size_t i = 0; i < host_patterns.size(); ++i) {
    std::string host = host_patterns[i].host();

    if (host_patterns[i].match_subdomains())
      host = "*." + host;

    std::string rcd;
    size_t reg_len = net::RegistryControlledDomainService::GetRegistryLength(
        host, false);
    if (reg_len && reg_len != std::string::npos) {
      if (include_rcd)  // else leave rcd empty
        rcd = host.substr(host.size() - reg_len);
      host = host.substr(0, host.size() - reg_len);
    }

    HostVector::iterator it = hosts_best_rcd.begin();
    for (; it != hosts_best_rcd.end(); ++it) {
      if (it->first == host)
        break;
    }
    if (it != hosts_best_rcd.end()) {
      if (include_rcd && RcdBetterThan(rcd, it->second))
        it->second = rcd;
    } else {  // Previously unseen host, append it.
      hosts_best_rcd.push_back(std::make_pair(host, rcd));
    }
  }

  std::vector<std::string> distinct_hosts;
  for (HostVector::iterator it = hosts_best_rcd.begin();
       it != hosts_best_rcd.end(); ++it)
    distinct_hosts.push_back(it->first + it->second);
  return distinct_hosts;
}
