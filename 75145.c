void SetExplicitlyAllowedPorts(const std::wstring& allowed_ports) {
  if (allowed_ports.empty())
    return;

  std::set<int> ports;
  size_t last = 0;
  size_t size = allowed_ports.size();
  const std::wstring::value_type kComma = L',';

  for (size_t i = 0; i <= size; ++i) {
    if (i != size && !IsAsciiDigit(allowed_ports[i]) &&
        (allowed_ports[i] != kComma))
      return;
    if (i == size || allowed_ports[i] == kComma) {
      size_t length = i - last;
      if (length > 0)
        ports.insert(StringToInt(WideToASCII(
            allowed_ports.substr(last, length))));
      last = i + 1;
    }
  }
  explicitly_allowed_ports = ports;
}
