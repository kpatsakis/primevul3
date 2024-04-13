void ExtensionService::UpdateActivePermissions(
    const Extension* extension,
    const ExtensionPermissionSet* permissions) {
  extension_prefs()->SetActivePermissions(extension->id(), permissions);
  extension->SetActivePermissions(permissions);
}
