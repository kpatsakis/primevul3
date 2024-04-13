Extension::PermissionMessages Extension::GetPermissionMessages() const {
  PermissionMessages messages;
  if (!plugins().empty()) {
    messages.push_back(PermissionMessage::CreateFromMessageId(
        PermissionMessage::ID_FULL_ACCESS));
    return messages;
  }

  if (HasEffectiveAccessToAllHosts()) {
    messages.push_back(PermissionMessage::CreateFromMessageId(
        PermissionMessage::ID_HOSTS_ALL));
  } else {
    std::vector<std::string> hosts = GetDistinctHostsForDisplay(
        GetEffectiveHostPermissions().patterns());
    if (!hosts.empty())
      messages.push_back(PermissionMessage::CreateFromHostList(hosts));
  }

  std::set<PermissionMessage> simple_msgs = GetSimplePermissionMessages();
  messages.insert(messages.end(), simple_msgs.begin(), simple_msgs.end());

  return messages;
}
