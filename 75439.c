bool ExtensionPrefs::ReadExtensionPrefList(
    const std::string& extension_id, const std::string& pref_key,
    const ListValue** out_value) {
  const DictionaryValue* ext = GetExtensionPref(extension_id);
  ListValue* out = NULL;
  if (!ext || !ext->GetList(pref_key, &out))
    return false;
  *out_value = out;

  return out_value != NULL;
}
