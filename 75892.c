bool Extension::HasApiPermission(
    const std::set<std::string>& api_permissions,
    const std::string& function_name) {
  std::string permission_name = function_name;

  for (size_t i = 0; i < kNumNonPermissionFunctionNames; ++i) {
    if (permission_name == kNonPermissionFunctionNames[i])
      return true;
  }

  size_t separator = function_name.find_first_of("./");
  if (separator != std::string::npos)
    permission_name = function_name.substr(0, separator);

  if (permission_name == kWindowPermission)
    permission_name = Extension::kTabPermission;

  if (api_permissions.count(permission_name))
    return true;

  for (size_t i = 0; i < kNumNonPermissionModuleNames; ++i) {
    if (permission_name == kNonPermissionModuleNames[i]) {
      return true;
    }
  }

  return false;
}
