bool ExtensionService::AllowFileAccess(const Extension* extension) {
  return (CommandLine::ForCurrentProcess()->HasSwitch(
              switches::kDisableExtensionsFileAccessCheck) ||
          extension_prefs_->AllowFileAccess(extension->id()));
}
