void ExtensionService::SetBrowserActionVisibility(const Extension* extension,
                                                  bool visible) {
  extension_prefs_->SetBrowserActionVisibility(extension, visible);
}
