void ExtensionService::InitializePermissions(const Extension* extension) {
  scoped_refptr<ExtensionPermissionSet> active_permissions =
      extension_prefs()->GetActivePermissions(extension->id());

  if (active_permissions.get()) {
    scoped_refptr<ExtensionPermissionSet> total_permissions =
        ExtensionPermissionSet::CreateUnion(
            extension->required_permission_set(),
            extension->optional_permission_set());

    scoped_refptr<ExtensionPermissionSet> adjusted_active =
        ExtensionPermissionSet::CreateIntersection(
            total_permissions.get(), active_permissions.get());

    adjusted_active = ExtensionPermissionSet::CreateUnion(
            extension->required_permission_set(), adjusted_active.get());

    UpdateActivePermissions(extension, adjusted_active);
  }

  const Extension* old = GetExtensionByIdInternal(extension->id(),
                                                  true, true, false);
  bool is_extension_upgrade = old != NULL;
  bool is_privilege_increase = false;

  if (!extension->CanSilentlyIncreasePermissions()) {
    scoped_refptr<ExtensionPermissionSet> granted_permissions =
        extension_prefs_->GetGrantedPermissions(extension->id());
    CHECK(granted_permissions.get());

    is_privilege_increase =
        granted_permissions->HasLessPrivilegesThan(
            extension->GetActivePermissions());
  }

  if (is_extension_upgrade) {
    if (extension->location() != Extension::LOAD)
      CHECK(extension->version()->CompareTo(*(old->version())) >= 0);

    if (!is_privilege_increase) {
      SetBeingUpgraded(old, true);
      SetBeingUpgraded(extension, true);
    }

    UnloadExtension(old->id(), extension_misc::UNLOAD_REASON_UPDATE);
    old = NULL;
  }

  if (is_privilege_increase) {
    if (!extension_prefs_->DidExtensionEscalatePermissions(extension->id())) {
      RecordPermissionMessagesHistogram(
          extension, "Extensions.Permissions_AutoDisable");
    }
    extension_prefs_->SetExtensionState(extension->id(), Extension::DISABLED);
    extension_prefs_->SetDidExtensionEscalatePermissions(extension, true);
  }
}
