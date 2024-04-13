bool ExtensionPrefs::GetGrantedPermissions(
    const std::string& extension_id,
    bool* full_access,
    std::set<std::string>* api_permissions,
    URLPatternSet* host_extent) {
  CHECK(Extension::IdIsValid(extension_id));

  const DictionaryValue* ext = GetExtensionPref(extension_id);
  if (!ext || !ext->GetBoolean(kPrefGrantedPermissionsAll, full_access))
    return false;

  ReadExtensionPrefStringSet(
      extension_id, kPrefGrantedPermissionsAPI, api_permissions);

  std::set<std::string> host_permissions;
  ReadExtensionPrefStringSet(
      extension_id, kPrefGrantedPermissionsHost, &host_permissions);
  bool allow_file_access = AllowFileAccess(extension_id);

  for (std::set<std::string>::iterator i = host_permissions.begin();
       i != host_permissions.end(); ++i) {
    URLPattern pattern(
        Extension::kValidHostPermissionSchemes |
        UserScript::kValidUserScriptSchemes);

    if (URLPattern::PARSE_SUCCESS != pattern.Parse(
            *i, URLPattern::PARSE_LENIENT)) {
      NOTREACHED();  // Corrupt prefs?  Hand editing?
    } else {
      if (!allow_file_access && pattern.MatchesScheme(chrome::kFileScheme)) {
        pattern.set_valid_schemes(
            pattern.valid_schemes() & ~URLPattern::SCHEME_FILE);
      }
      host_extent->AddPattern(pattern);
    }
  }

  return true;
}
