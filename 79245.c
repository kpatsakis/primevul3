void TabsCaptureVisibleTabFunction::RegisterUserPrefs(
    PrefRegistrySyncable* registry) {
  registry->RegisterBooleanPref(prefs::kDisableScreenshots, false,
                                PrefRegistrySyncable::UNSYNCABLE_PREF);
}
