void ExtensionPrefs::UpdateExtensionPref(const std::string& extension_id,
                                         const std::string& key,
                                         Value* data_value) {
  if (!Extension::IdIsValid(extension_id)) {
    NOTREACHED() << "Invalid extension_id " << extension_id;
    return;
  }
  ScopedExtensionPrefUpdate update(prefs_, extension_id);
  update->Set(key, data_value);
}
