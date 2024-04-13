void ExtensionPrefs::DeleteExtensionPrefs(const std::string& extension_id) {
  DictionaryPrefUpdate update(prefs_, kExtensionsPref);
  DictionaryValue* dict = update.Get();
  if (dict->HasKey(extension_id)) {
    dict->Remove(extension_id, NULL);
    SavePrefs();
  }
  extension_pref_value_map_->UnregisterExtension(extension_id);
  content_settings_store_->UnregisterExtension(extension_id);
}
