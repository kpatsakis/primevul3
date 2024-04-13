void AutoFillManager::RegisterBrowserPrefs(PrefService* prefs) {
  prefs->RegisterDictionaryPref(prefs::kAutoFillDialogPlacement);
}
