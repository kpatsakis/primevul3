void ExtensionPrefs::InitPrefStore() {
  ExtensionIdSet extension_ids;
  GetExtensions(&extension_ids);
  for (ExtensionIdSet::iterator ext_id = extension_ids.begin();
       ext_id != extension_ids.end(); ++ext_id) {
    ScopedExtensionPrefUpdate update(prefs_, *ext_id);
    update.Get();
  }

  FixMissingPrefs(extension_ids);
  for (ExtensionIdSet::iterator ext_id = extension_ids.begin();
       ext_id != extension_ids.end(); ++ext_id) {
    extension_pref_value_map_->RegisterExtension(
        *ext_id,
        GetInstallTime(*ext_id),
        GetExtensionState(*ext_id) == Extension::ENABLED);
    content_settings_store_->RegisterExtension(
        *ext_id,
        GetInstallTime(*ext_id),
        GetExtensionState(*ext_id) == Extension::ENABLED);

    const DictionaryValue* prefs = GetExtensionControlledPrefs(*ext_id, false);
    for (DictionaryValue::key_iterator i = prefs->begin_keys();
         i != prefs->end_keys(); ++i) {
      Value* value;
      if (!prefs->GetWithoutPathExpansion(*i, &value))
        continue;
      extension_pref_value_map_->SetExtensionPref(
          *ext_id, *i, kExtensionPrefsScopeRegular, value->DeepCopy());
    }

    prefs = GetExtensionControlledPrefs(*ext_id, true);
    for (DictionaryValue::key_iterator i = prefs->begin_keys();
         i != prefs->end_keys(); ++i) {
      Value* value;
      if (!prefs->GetWithoutPathExpansion(*i, &value))
        continue;
      extension_pref_value_map_->SetExtensionPref(
          *ext_id, *i, kExtensionPrefsScopeIncognitoPersistent,
          value->DeepCopy());
    }

    const DictionaryValue* extension_prefs = GetExtensionPref(*ext_id);
    DCHECK(extension_prefs);
    ListValue* content_settings = NULL;
    if (extension_prefs->GetList(kPrefContentSettings,
                                 &content_settings)) {
      content_settings_store_->SetExtensionContentSettingsFromList(
          *ext_id, content_settings,
          kExtensionPrefsScopeRegular);
    }
    if (extension_prefs->GetList(kPrefIncognitoContentSettings,
                                 &content_settings)) {
      content_settings_store_->SetExtensionContentSettingsFromList(
          *ext_id, content_settings,
          kExtensionPrefsScopeIncognitoPersistent);
    }
  }

  extension_pref_value_map_->NotifyInitializationCompleted();
}
