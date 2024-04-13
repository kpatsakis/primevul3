  void GrantPermissionsForFileSystem(const std::string& filesystem_id,
                                     int permissions) {
    if (filesystem_permissions_.find(filesystem_id) ==
        filesystem_permissions_.end())
      fileapi::IsolatedContext::GetInstance()->AddReference(filesystem_id);
    filesystem_permissions_[filesystem_id] = permissions;
  }
