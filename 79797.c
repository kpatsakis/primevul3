void ChromeDownloadManagerDelegate::GetSaveDir(BrowserContext* browser_context,
                                               FilePath* website_save_dir,
                                               FilePath* download_save_dir,
                                               bool* skip_dir_check) {
  Profile* profile = Profile::FromBrowserContext(browser_context);
  PrefServiceSyncable* prefs = profile->GetPrefs();

  if (!prefs->FindPreference(prefs::kSaveFileDefaultDirectory)) {
    DCHECK(prefs->FindPreference(prefs::kDownloadDefaultDirectory));
    FilePath default_save_path = prefs->GetFilePath(
        prefs::kDownloadDefaultDirectory);
    prefs->RegisterFilePathPref(prefs::kSaveFileDefaultDirectory,
                                default_save_path,
                                PrefServiceSyncable::UNSYNCABLE_PREF);
  }

  *website_save_dir = prefs->GetFilePath(prefs::kSaveFileDefaultDirectory);
  DCHECK(!website_save_dir->empty());

  *download_save_dir = prefs->GetFilePath(prefs::kDownloadDefaultDirectory);

  *skip_dir_check = false;
#if defined(OS_CHROMEOS)
  *skip_dir_check = drive::util::IsUnderDriveMountPoint(*website_save_dir);
#endif
}
