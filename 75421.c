void ExtensionPrefs::AddGrantedPermissions(
    const std::string& extension_id,
    const bool full_access,
    const std::set<std::string>& api_permissions,
    const URLPatternSet& host_extent) {
  CHECK(Extension::IdIsValid(extension_id));

  UpdateExtensionPref(extension_id, kPrefGrantedPermissionsAll,
                      Value::CreateBooleanValue(full_access));

  if (!api_permissions.empty()) {
    AddToExtensionPrefStringSet(
        extension_id, kPrefGrantedPermissionsAPI, api_permissions);
  }

  if (!host_extent.is_empty()) {
    std::set<std::string> host_permissions;
    ExtentToStringSet(host_extent, &host_permissions);

    AddToExtensionPrefStringSet(
        extension_id, kPrefGrantedPermissionsHost, host_permissions);
  }
}
