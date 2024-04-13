  bool HasPermissionsForFileSystem(const std::string& filesystem_id,
                                   int permissions) {
    if (filesystem_permissions_.find(filesystem_id) ==
        filesystem_permissions_.end())
      return false;
    return (filesystem_permissions_[filesystem_id] & permissions) ==
        permissions;
  }
