bool ExtensionService::IsExternalExtensionUninstalled(
    const std::string& extension_id) const {
  return extension_prefs_->IsExternalExtensionUninstalled(extension_id);
}
