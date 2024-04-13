bool ExtensionService::GetBrowserActionVisibility(const Extension* extension) {
  return extension_prefs_->GetBrowserActionVisibility(extension);
}
