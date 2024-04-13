std::vector<string16> Extension::GetPermissionMessageStrings() const {
  std::vector<string16> messages;
  PermissionMessages permissions = GetPermissionMessages();
  for (PermissionMessages::const_iterator i = permissions.begin();
       i != permissions.end(); ++i)
    messages.push_back(i->message());
  return messages;
}
