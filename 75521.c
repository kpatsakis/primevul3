void ExtensionService::GrantPermissions(const Extension* extension) {
  CHECK(extension);

  if (extension->CanSilentlyIncreasePermissions())
    return;

  extension_prefs_->AddGrantedPermissions(extension->id(),
                                          extension->GetActivePermissions());
}
