bool ExtensionPrefs::IsExternalExtensionUninstalled(
    const std::string& id) const {
  const DictionaryValue* extension = GetExtensionPref(id);
  if (!extension)
    return false;
  int state = 0;
  return extension->GetInteger(kPrefState, &state) &&
         state == static_cast<int>(Extension::EXTERNAL_EXTENSION_UNINSTALLED);
}
