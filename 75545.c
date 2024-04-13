void ExtensionService::LoadInstalledExtension(const ExtensionInfo& info,
                                              bool write_to_prefs) {
  std::string error;
  scoped_refptr<const Extension> extension(NULL);
  if (!extension_prefs_->IsExtensionAllowedByPolicy(info.extension_id)) {
    error = errors::kDisabledByPolicy;
  } else if (info.extension_manifest.get()) {
    int flags = Extension::NO_FLAGS;
    if (info.extension_location != Extension::LOAD)
      flags |= Extension::REQUIRE_KEY;
    if (Extension::ShouldDoStrictErrorChecking(info.extension_location))
      flags |= Extension::STRICT_ERROR_CHECKS;
    if (extension_prefs_->AllowFileAccess(info.extension_id))
      flags |= Extension::ALLOW_FILE_ACCESS;
    if (extension_prefs_->IsFromWebStore(info.extension_id))
      flags |= Extension::FROM_WEBSTORE;
    if (extension_prefs_->IsFromBookmark(info.extension_id))
      flags |= Extension::FROM_BOOKMARK;
    extension = Extension::Create(
        info.extension_path,
        info.extension_location,
        *info.extension_manifest,
        flags,
        &error);
  } else {
    error = errors::kManifestUnreadable;
  }

  if (!extension) {
    ReportExtensionLoadError(info.extension_path,
                             error,
                             chrome::NOTIFICATION_EXTENSION_INSTALL_ERROR,
                             false);
    return;
  }

  if (write_to_prefs)
    extension_prefs_->UpdateManifest(extension);

  AddExtension(extension);
}
