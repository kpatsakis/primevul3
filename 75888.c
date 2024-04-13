Extension::PermissionMessage::MessageId Extension::GetPermissionMessageId(
    const std::string& permission) {
  return ExtensionConfig::GetInstance()->GetPermissionMessageId(permission);
}
