int ExtensionPrefs::GetPageIndex(const std::string& extension_id) {
  int value = -1;
  ReadExtensionPrefInteger(extension_id, kPrefPageIndex, &value);
  return value;
}
