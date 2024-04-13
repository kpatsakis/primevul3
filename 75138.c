bool IsPortAllowedByOverride(int port) {
  if (explicitly_allowed_ports.empty())
    return false;

  std::set<int>::const_iterator it =
      std::find(explicitly_allowed_ports.begin(),
                explicitly_allowed_ports.end(),
                port);

  return it != explicitly_allowed_ports.end();
}
