bool ExtensionPrefs::ReadExtensionPrefBoolean(
    const std::string& extension_id, const std::string& pref_key) {
  const DictionaryValue* ext = GetExtensionPref(extension_id);
  if (!ext) {
    return false;
  }
  return ReadBooleanFromPref(ext, pref_key);
}
