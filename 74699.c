void AutoFillManager::RegisterUserPrefs(PrefService* prefs) {
  prefs->RegisterBooleanPref(prefs::kAutoFillEnabled, true);
#if defined(OS_MACOSX)
  prefs->RegisterBooleanPref(prefs::kAutoFillAuxiliaryProfilesEnabled, true);
#else
  prefs->RegisterBooleanPref(prefs::kAutoFillAuxiliaryProfilesEnabled, false);
#endif
  prefs->RegisterRealPref(prefs::kAutoFillPositiveUploadRate,
                          kAutoFillPositiveUploadRateDefaultValue);
  prefs->RegisterRealPref(prefs::kAutoFillNegativeUploadRate,
                          kAutoFillNegativeUploadRateDefaultValue);
}
