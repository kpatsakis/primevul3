int ExtensionPrefs::GetNextAppLaunchIndex() {
  const DictionaryValue* extensions = prefs_->GetDictionary(kExtensionsPref);
  if (!extensions)
    return 0;

  int max_value = -1;
  for (DictionaryValue::key_iterator extension_id = extensions->begin_keys();
       extension_id != extensions->end_keys(); ++extension_id) {
    int value = GetAppLaunchIndex(*extension_id);
    if (value > max_value)
      max_value = value;
  }
  return max_value + 1;
}
