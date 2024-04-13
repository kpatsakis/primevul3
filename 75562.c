void ExtensionService::ReloadExtension(const std::string& extension_id) {
  CHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  FilePath path;
  const Extension* current_extension = GetExtensionById(extension_id, false);

  if (current_extension) {
    ExtensionProcessManager* manager = profile_->GetExtensionProcessManager();
    ExtensionHost* host = manager->GetBackgroundHostForExtension(
        current_extension);
    if (host) {
      int devtools_cookie = DevToolsManager::GetInstance()->DetachClientHost(
          host->render_view_host());
      if (devtools_cookie >= 0)
        orphaned_dev_tools_[extension_id] = devtools_cookie;
    }

    path = current_extension->path();
    DisableExtension(extension_id);
    disabled_extension_paths_[extension_id] = path;
  } else {
    path = unloaded_extension_paths_[extension_id];
  }

  scoped_ptr<ExtensionInfo> installed_extension(
      extension_prefs_->GetInstalledExtensionInfo(extension_id));
  if (installed_extension.get() &&
      installed_extension->extension_manifest.get()) {
    LoadInstalledExtension(*installed_extension, false);
  } else {
    CHECK(!path.empty());
    LoadExtension(path);
  }
}
