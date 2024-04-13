std::wstring FormatViewSourceUrl(const GURL& url,
                                 const std::wstring& languages,
                                 bool omit_username_password,
                                 UnescapeRule::Type unescape_rules,
                                 url_parse::Parsed* new_parsed,
                                 size_t* prefix_end,
                                 size_t* offset_for_adjustment) {
  DCHECK(new_parsed);
  const wchar_t* const kWideViewSource = L"view-source:";
  const size_t kViewSourceLengthPlus1 = 12;

  GURL real_url(url.possibly_invalid_spec().substr(kViewSourceLengthPlus1));
  size_t temp_offset = (*offset_for_adjustment == std::wstring::npos) ?
      std::wstring::npos : (*offset_for_adjustment - kViewSourceLengthPlus1);
  size_t* temp_offset_ptr = (*offset_for_adjustment < kViewSourceLengthPlus1) ?
      NULL : &temp_offset;
  std::wstring result = net::FormatUrl(real_url, languages,
      omit_username_password, unescape_rules, new_parsed, prefix_end,
      temp_offset_ptr);
  result.insert(0, kWideViewSource);

  if (new_parsed->scheme.is_nonempty()) {
    new_parsed->scheme.len += kViewSourceLengthPlus1;
  } else {
    new_parsed->scheme.begin = 0;
    new_parsed->scheme.len = kViewSourceLengthPlus1 - 1;
  }
  if (new_parsed->username.is_nonempty())
    new_parsed->username.begin += kViewSourceLengthPlus1;
  if (new_parsed->password.is_nonempty())
    new_parsed->password.begin += kViewSourceLengthPlus1;
  if (new_parsed->host.is_nonempty())
    new_parsed->host.begin += kViewSourceLengthPlus1;
  if (new_parsed->port.is_nonempty())
    new_parsed->port.begin += kViewSourceLengthPlus1;
  if (new_parsed->path.is_nonempty())
    new_parsed->path.begin += kViewSourceLengthPlus1;
  if (new_parsed->query.is_nonempty())
    new_parsed->query.begin += kViewSourceLengthPlus1;
  if (new_parsed->ref.is_nonempty())
    new_parsed->ref.begin += kViewSourceLengthPlus1;
  if (prefix_end)
    *prefix_end += kViewSourceLengthPlus1;
  if (temp_offset_ptr) {
    *offset_for_adjustment = (temp_offset == std::wstring::npos) ?
        std::wstring::npos : (temp_offset + kViewSourceLengthPlus1);
  }
  return result;
}
