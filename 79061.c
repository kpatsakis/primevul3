bool HeadersContainMultipleCopiesOfField(
    const net::HttpResponseHeaders& headers,
    const std::string& field_name) {
  void* it = NULL;
  std::string field_value;
  if (!headers.EnumerateHeader(&it, field_name, &field_value))
    return false;
  std::string field_value2;
  while (headers.EnumerateHeader(&it, field_name, &field_value2)) {
    if (field_value != field_value2)
      return true;
  }
  return false;
}
