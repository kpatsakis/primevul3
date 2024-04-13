bool Extension::IsAPIPermission(const std::string& str) const {
  for (size_t i = 0; i < Extension::kNumPermissions; ++i) {
    if (str == Extension::kPermissions[i].name) {
      return true;
    }
  }
  return false;
}
