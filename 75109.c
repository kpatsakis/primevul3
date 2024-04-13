bool DecodeParamValue(const std::string& input,
                      const std::string& referrer_charset,
                      std::string* output) {
  std::string tmp;
  StringTokenizer t(input, " \t\n\r");
  t.set_options(StringTokenizer::RETURN_DELIMS);
  bool is_previous_token_rfc2047 = true;
  while (t.GetNext()) {
    if (t.token_is_delim()) {
      if (!is_previous_token_rfc2047) {
        tmp.push_back(' ');
      }
      continue;
    }
    std::string decoded;
    if (!DecodeWord(t.token(), referrer_charset, &is_previous_token_rfc2047,
                    &decoded))
      return false;
    tmp.append(decoded);
  }
  output->swap(tmp);
  return true;
}
