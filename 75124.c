STR GetSpecificHeaderT(const STR& headers, const STR& name) {
  if (headers.empty())
    return STR();

  STR match;
  match.push_back('\n');
  match.append(name);
  match.push_back(':');

  typename STR::const_iterator begin =
      search(headers.begin(), headers.end(), match.begin(), match.end(),
             CaseInsensitiveCompareASCII<typename STR::value_type>());

  if (begin == headers.end())
    return STR();

  begin += match.length();

  typename STR::const_iterator end = find(begin, headers.end(), '\n');

  STR ret;
  TrimWhitespace(STR(begin, end), TRIM_ALL, &ret);
  return ret;
}
