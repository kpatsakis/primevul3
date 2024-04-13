static void CleanupBadExtensionKeys(const FilePath& root_dir,
                                    PrefService* prefs) {
  const DictionaryValue* dictionary =
      prefs->GetDictionary(ExtensionPrefs::kExtensionsPref);
  std::map<std::string, std::string> remapped_keys;
  for (DictionaryValue::key_iterator i = dictionary->begin_keys();
       i != dictionary->end_keys(); ++i) {
    DictionaryValue* ext;
    if (!dictionary->GetDictionaryWithoutPathExpansion(*i, &ext))
      continue;

    int location;
    FilePath::StringType path_str;
    if (!ext->GetInteger(kPrefLocation, &location) ||
        !ext->GetString(kPrefPath, &path_str)) {
      continue;
    }

    if (location != Extension::LOAD)
      continue;

    const std::string& prefs_id(*i);
    FilePath path(path_str);
    if (!path.IsAbsolute()) {
      path = root_dir.Append(path);
    }
    std::string computed_id = Extension::GenerateIdForPath(path);

    if (prefs_id != computed_id) {
      remapped_keys[prefs_id] = computed_id;
    }
  }

  if (!remapped_keys.empty()) {
    DictionaryPrefUpdate update(prefs, ExtensionPrefs::kExtensionsPref);
    DictionaryValue* update_dictionary = update.Get();
    for (std::map<std::string, std::string>::const_iterator i =
            remapped_keys.begin();
        i != remapped_keys.end();
        ++i) {
      if (update_dictionary->HasKey(i->second)) {
        CHECK(update_dictionary->RemoveWithoutPathExpansion(i->first, NULL));
        continue;
      }
      Value* extension_prefs = NULL;
      CHECK(update_dictionary->RemoveWithoutPathExpansion(
          i->first, &extension_prefs));
      update_dictionary->SetWithoutPathExpansion(i->second, extension_prefs);
    }

    prefs->ScheduleSavePersistentPrefs();
  }
}
