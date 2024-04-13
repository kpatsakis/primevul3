Extension::PermissionMessage Extension::PermissionMessage::CreateFromMessageId(
    Extension::PermissionMessage::MessageId message_id) {
  DCHECK_GT(PermissionMessage::ID_NONE, PermissionMessage::ID_UNKNOWN);
  if (message_id <= ID_NONE)
    return PermissionMessage(message_id, string16());

  string16 message = l10n_util::GetStringUTF16(kMessageIds[message_id]);
  return PermissionMessage(message_id, message);
}
