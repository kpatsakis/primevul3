void ExtensionService::GarbageCollectExtensions() {
  if (extension_prefs_->pref_service()->ReadOnly())
    return;

  scoped_ptr<ExtensionPrefs::ExtensionsInfo> info(
      extension_prefs_->GetInstalledExtensionsInfo());

  std::map<std::string, FilePath> extension_paths;
  for (size_t i = 0; i < info->size(); ++i)
    extension_paths[info->at(i)->extension_id] = info->at(i)->extension_path;

  if (!BrowserThread::PostTask(
          BrowserThread::FILE, FROM_HERE,
          NewRunnableFunction(
              &extension_file_util::GarbageCollectExtensions,
              install_directory_,
              extension_paths)))
    NOTREACHED();

  if (profile_) {
    ThemeServiceFactory::GetForProfile(profile_)->RemoveUnusedThemes();
  }
}
