void ExtensionPrefs::SetPageIndex(const std::string& extension_id, int index) {
  CHECK_GE(index, 0);
  UpdateExtensionPref(extension_id, kPrefPageIndex,
                      Value::CreateIntegerValue(index));
}
