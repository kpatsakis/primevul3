void ExtensionService::ProcessExtensionSyncData(
    const ExtensionSyncData& extension_sync_data,
    SyncBundle& bundle) {
  const std::string& id = extension_sync_data.id();

  if (extension_sync_data.uninstalled()) {
    std::string error;
    if (!UninstallExtensionHelper(this, id)) {
      LOG(WARNING) << "Could not uninstall extension " << id
                   << " for sync";
    }
    return;
  }

  if (extension_sync_data.enabled()) {
    EnableExtension(id);
  } else {
    DisableExtension(id);
  }
  SetIsIncognitoEnabled(id, extension_sync_data.incognito_enabled());

  const Extension* extension = GetInstalledExtension(id);
  if (extension) {
    int result = extension->version()->CompareTo(extension_sync_data.version());
    if (result < 0) {
      bundle.pending_sync_data[extension_sync_data.id()] = extension_sync_data;
      CheckForUpdatesSoon();
    }
  } else {
    const bool kInstallSilently = true;
    if (!pending_extension_manager()->AddFromSync(
            id,
            extension_sync_data.update_url(),
            bundle.filter,
            kInstallSilently)) {
      LOG(WARNING) << "Could not add pending extension for " << id;
    }
    bundle.pending_sync_data[extension_sync_data.id()] = extension_sync_data;
    CheckForUpdatesSoon();
  }
}
