void BookmarkManagerView::RegisterPrefs(PrefService* prefs) {
  prefs->RegisterDictionaryPref(prefs::kBookmarkManagerPlacement);
  prefs->RegisterIntegerPref(prefs::kBookmarkManagerSplitLocation, -1);
}
