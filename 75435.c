void ExtensionPrefs::MakePathsAbsolute(DictionaryValue* dict) {
  if (!dict || dict->empty())
    return;

  for (DictionaryValue::key_iterator i = dict->begin_keys();
       i != dict->end_keys(); ++i) {
    DictionaryValue* extension_dict = NULL;
    if (!dict->GetDictionaryWithoutPathExpansion(*i, &extension_dict)) {
      NOTREACHED();
      continue;
    }

    int location_value;
    if (extension_dict->GetInteger(kPrefLocation, &location_value) &&
        location_value == Extension::LOAD) {
      continue;
    }

    FilePath::StringType path_string;
    if (!extension_dict->GetString(kPrefPath, &path_string))
      continue;

    DCHECK(!FilePath(path_string).IsAbsolute());
    extension_dict->SetString(
        kPrefPath, install_directory_.Append(path_string).value());
  }
}
