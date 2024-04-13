bool ExecuteCodeInTabFunction::HasPermission() {
  if (Init() &&
      extension_->HasAPIPermissionForTab(execute_tab_id_,
                                         extensions::APIPermission::kTab)) {
    return true;
  }
  return ExtensionFunction::HasPermission();
}
