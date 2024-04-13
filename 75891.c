    Extension::GetSimplePermissionMessages() const {
  std::set<PermissionMessage> messages;
  std::set<std::string>::const_iterator i;
  for (i = api_permissions().begin(); i != api_permissions().end(); ++i) {
    PermissionMessage::MessageId message_id = GetPermissionMessageId(*i);
    DCHECK_GT(PermissionMessage::ID_NONE, PermissionMessage::ID_UNKNOWN);
    if (message_id > PermissionMessage::ID_NONE)
      messages.insert(PermissionMessage::CreateFromMessageId(message_id));
  }
  return messages;
}
