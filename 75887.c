  Extension::PermissionMessage::MessageId GetPermissionMessageId(
      const std::string& permission) {
    return Extension::kPermissions[permission_map_[permission]].message_id;
  }
