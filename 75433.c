bool ExtensionPrefs::IsExtensionAllowedByPolicy(
    const std::string& extension_id) {
  std::string string_value;

  const ListValue* blacklist =
      prefs_->GetList(prefs::kExtensionInstallDenyList);
  if (!blacklist || blacklist->empty())
    return true;

  const ListValue* whitelist =
      prefs_->GetList(prefs::kExtensionInstallAllowList);
  if (whitelist) {
    for (ListValue::const_iterator it = whitelist->begin();
         it != whitelist->end(); ++it) {
      if (!(*it)->GetAsString(&string_value))
        LOG(WARNING) << "Failed to read whitelist string.";
      else if (string_value == extension_id)
        return true;
    }
  }

  if (blacklist) {
    for (ListValue::const_iterator it = blacklist->begin();
         it != blacklist->end(); ++it) {
      if (!(*it)->GetAsString(&string_value)) {
        LOG(WARNING) << "Failed to read blacklist string.";
      } else {
        if (string_value == "*")
          return false;  // Only whitelisted extensions are allowed.
        if (string_value == extension_id)
          return false;
      }
    }
  }

  return true;
}
