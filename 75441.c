void ExtensionPrefs::SetAppLaunchIndex(const std::string& extension_id,
                                       int index) {
  DCHECK_GE(index, 0);
  UpdateExtensionPref(extension_id, kPrefAppLaunchIndex,
                      Value::CreateIntegerValue(index));
}
