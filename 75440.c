bool ExtensionPrefs::ReadExtensionPrefStringSet(
    const std::string& extension_id,
    const std::string& pref_key,
    std::set<std::string>* result) {
  const ListValue* value = NULL;
  if (!ReadExtensionPrefList(extension_id, pref_key, &value))
    return false;

  result->clear();

  for (size_t i = 0; i < value->GetSize(); ++i) {
    std::string item;
    if (!value->GetString(i, &item))
      return false;
    result->insert(item);
  }

  return true;
}
