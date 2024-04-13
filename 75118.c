STR GetHeaderParamValueT(const STR& header, const STR& param_name) {
  typename STR::const_iterator param_begin =
      search(header.begin(), header.end(), param_name.begin(), param_name.end(),
             CaseInsensitiveCompareASCII<typename STR::value_type>());

  if (param_begin == header.end())
    return STR();
  param_begin += param_name.length();

  STR whitespace;
  whitespace.push_back(' ');
  whitespace.push_back('\t');
  const typename STR::size_type equals_offset =
      header.find_first_not_of(whitespace, param_begin - header.begin());
  if (equals_offset == STR::npos || header.at(equals_offset) != '=')
    return STR();

  param_begin = header.begin() + equals_offset + 1;
  if (param_begin == header.end())
    return STR();

  typename STR::const_iterator param_end;
  if (*param_begin == '"') {
    param_end = find(param_begin+1, header.end(), '"');
    if (param_end == header.end())
      return STR();  // poorly formatted param?

    ++param_begin;  // skip past the quote.
  } else {
    param_end = find(param_begin+1, header.end(), ';');
  }

  return STR(param_begin, param_end);
}
