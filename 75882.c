Extension::PermissionMessage Extension::PermissionMessage::CreateFromHostList(
    const std::vector<std::string> hosts) {
  CHECK(hosts.size() > 0);

  MessageId message_id;
  string16 message;
  switch (hosts.size()) {
    case 1:
      message_id = ID_HOSTS_1;
      message = l10n_util::GetStringFUTF16(kMessageIds[message_id],
                                           UTF8ToUTF16(hosts[0]));
      break;
    case 2:
      message_id = ID_HOSTS_2;
      message = l10n_util::GetStringFUTF16(kMessageIds[message_id],
                                           UTF8ToUTF16(hosts[0]),
                                           UTF8ToUTF16(hosts[1]));
      break;
    case 3:
      message_id = ID_HOSTS_3;
      message = l10n_util::GetStringFUTF16(kMessageIds[message_id],
                                           UTF8ToUTF16(hosts[0]),
                                           UTF8ToUTF16(hosts[1]),
                                           UTF8ToUTF16(hosts[2]));
      break;
    default:
      message_id = ID_HOSTS_4_OR_MORE;
      message = l10n_util::GetStringFUTF16(
          kMessageIds[message_id],
          UTF8ToUTF16(hosts[0]),
          UTF8ToUTF16(hosts[1]),
          base::IntToString16(hosts.size() - 2));
      break;
  }

  return PermissionMessage(message_id, message);
}
