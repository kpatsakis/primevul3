bool Extension::IsComponentOnlyPermission(const std::string& permission) const {
  if (location() == Extension::COMPONENT)
    return true;

  for (size_t i = 0; i < Extension::kNumComponentPrivatePermissions; ++i) {
    if (permission == Extension::kComponentPrivatePermissionNames[i])
      return false;
  }
  return true;
}
