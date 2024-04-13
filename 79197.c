void DevToolsWindow::RegisterProfilePrefs(
    user_prefs::PrefRegistrySyncable* registry) {
  registry->RegisterBooleanPref(
      prefs::kDevToolsOpenDocked, true,
      user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF);
  registry->RegisterStringPref(
      prefs::kDevToolsDockSide, kDockSideBottom,
      user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF);
  registry->RegisterDictionaryPref(
      prefs::kDevToolsEditedFiles,
      user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF);
  registry->RegisterDictionaryPref(
      prefs::kDevToolsFileSystemPaths,
      user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF);
  registry->RegisterStringPref(
      prefs::kDevToolsAdbKey, std::string(),
      user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF);

  registry->RegisterDictionaryPref(
      GetDevToolsWindowPlacementPrefKey().c_str(),
      user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF);

  registry->RegisterBooleanPref(
      prefs::kDevToolsDiscoverUsbDevicesEnabled,
      false,
      user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF);
  registry->RegisterBooleanPref(
      prefs::kDevToolsPortForwardingEnabled,
      false,
      user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF);
  registry->RegisterBooleanPref(
      prefs::kDevToolsPortForwardingDefaultSet,
      false,
      user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF);
  registry->RegisterDictionaryPref(
      prefs::kDevToolsPortForwardingConfig,
      user_prefs::PrefRegistrySyncable::UNSYNCABLE_PREF);
}
