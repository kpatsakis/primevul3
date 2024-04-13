bool Extension::IsPrivilegeIncrease(const bool granted_full_access,
                                    const std::set<std::string>& granted_apis,
                                    const URLPatternSet& granted_extent,
                                    const Extension* new_extension) {
  if (granted_full_access)
    return false;

  if (new_extension->HasFullPermissions())
    return true;

  if (!HasEffectiveAccessToAllHosts(granted_extent, granted_apis)) {
    if (new_extension->HasEffectiveAccessToAllHosts())
      return true;

    const URLPatternSet new_extent =
        new_extension->GetEffectiveHostPermissions();

    if (IsElevatedHostList(granted_extent.patterns(), new_extent.patterns()))
      return true;
  }

  std::set<std::string> new_apis = new_extension->api_permissions();
  std::set<std::string> new_apis_only;
  std::set_difference(new_apis.begin(), new_apis.end(),
                      granted_apis.begin(), granted_apis.end(),
                      std::inserter(new_apis_only, new_apis_only.begin()));

  size_t new_api_count = 0;
  for (std::set<std::string>::iterator i = new_apis_only.begin();
       i != new_apis_only.end(); ++i) {
    DCHECK_GT(PermissionMessage::ID_NONE, PermissionMessage::ID_UNKNOWN);
    if (GetPermissionMessageId(*i) > PermissionMessage::ID_NONE)
      new_api_count++;
  }

  if (new_api_count)
    return true;

  return false;
}
