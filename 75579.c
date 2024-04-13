bool ExtensionService::UninstallExtension(
    const std::string& extension_id_unsafe,
    bool external_uninstall,
    std::string* error) {
  CHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  std::string extension_id(extension_id_unsafe);

  const Extension* extension = GetInstalledExtension(extension_id);

  CHECK(extension);

  GURL extension_url(extension->url());
  Extension::Location location(extension->location());

  if (!Extension::UserMayDisable(location) && !external_uninstall) {
    NotificationService::current()->Notify(
        chrome::NOTIFICATION_EXTENSION_UNINSTALL_NOT_ALLOWED,
        Source<Profile>(profile_),
        Details<const Extension>(extension));
    if (error != NULL) {
      *error = errors::kCannotUninstallManagedExtension;
    }
    return false;
  }

  SyncBundle* sync_bundle = GetSyncBundleForExtension(*extension);

  SyncChange sync_change;
  if (sync_bundle) {
    ExtensionSyncData extension_sync_data(*extension,
                                          IsExtensionEnabled(extension_id),
                                          IsIncognitoEnabled(extension_id));
    sync_change = extension_sync_data.GetSyncChange(SyncChange::ACTION_DELETE);
  }

  UninstalledExtensionInfo uninstalled_extension_info(*extension);

  UMA_HISTOGRAM_ENUMERATION("Extensions.UninstallType",
                            extension->GetType(), 100);
  RecordPermissionMessagesHistogram(
      extension, "Extensions.Permissions_Uninstall");

  TemplateURLService* url_service =
      TemplateURLServiceFactory::GetForProfile(profile_);
  if (url_service)
    url_service->UnregisterExtensionKeyword(extension);

  UnloadExtension(extension_id, extension_misc::UNLOAD_REASON_UNINSTALL);

  extension_prefs_->OnExtensionUninstalled(extension_id, location,
                                           external_uninstall);

  if (Extension::LOAD != location) {
    if (!BrowserThread::PostTask(
            BrowserThread::FILE, FROM_HERE,
            NewRunnableFunction(
                &extension_file_util::UninstallExtension,
                install_directory_,
                extension_id)))
      NOTREACHED();
  }

  ClearExtensionData(extension_url);
  UntrackTerminatedExtension(extension_id);

  NotificationService::current()->Notify(
      chrome::NOTIFICATION_EXTENSION_UNINSTALLED,
      Source<Profile>(profile_),
      Details<UninstalledExtensionInfo>(&uninstalled_extension_info));

  if (sync_bundle && sync_bundle->HasExtensionId(extension_id)) {
    sync_bundle->sync_processor->ProcessSyncChanges(
        FROM_HERE, SyncChangeList(1, sync_change));
    sync_bundle->synced_extensions.erase(extension_id);
  }

  return true;
}
