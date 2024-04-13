Extension::State ExtensionPrefs::GetExtensionState(
    const std::string& extension_id) const {
  const DictionaryValue* extension = GetExtensionPref(extension_id);

  if (!extension)
    return Extension::ENABLED;

  int state = -1;
  if (!extension->GetInteger(kPrefState, &state) ||
      state < 0 || state >= Extension::NUM_STATES) {
    LOG(ERROR) << "Bad or missing pref 'state' for extension '"
               << extension_id << "'";
    return Extension::ENABLED;
  }
  return static_cast<Extension::State>(state);
}
