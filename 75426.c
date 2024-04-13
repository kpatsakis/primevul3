int ExtensionPrefs::GetAppLaunchIndex(const std::string& extension_id) {
  int value;
  if (ReadExtensionPrefInteger(extension_id, kPrefAppLaunchIndex, &value))
    return value;

  return -1;
}
