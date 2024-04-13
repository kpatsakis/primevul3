void ExtensionService::GrantPermissionsAndEnableExtension(
    const Extension* extension) {
  CHECK(extension);
  RecordPermissionMessagesHistogram(
      extension, "Extensions.Permissions_ReEnable");
  GrantPermissions(extension);
  extension_prefs_->SetDidExtensionEscalatePermissions(extension, false);
  EnableExtension(extension->id());
}
