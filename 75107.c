std::string CanonicalizeHost(const std::wstring& host,
                             url_canon::CanonHostInfo* host_info) {
  std::string converted_host;
  WideToUTF8(host.c_str(), host.length(), &converted_host);
  return CanonicalizeHost(converted_host, host_info);
}
