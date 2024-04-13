std::string GetFileNameFromCD(const std::string& header,
                              const std::string& referrer_charset) {
  std::string param_value = GetHeaderParamValue(header, "filename");
  if (param_value.empty()) {
    param_value = GetHeaderParamValue(header, "name");
  }
  if (param_value.empty())
    return std::string();
  std::string decoded;
  if (DecodeParamValue(param_value, referrer_charset, &decoded))
    return decoded;
  return std::string();
}
