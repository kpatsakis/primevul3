std::string URLFixerUpper::SegmentURL(const std::string& text,
                                      url_parse::Parsed* parts) {
  *parts = url_parse::Parsed();

  std::string trimmed;
  TrimWhitespaceUTF8(text, TRIM_ALL, &trimmed);
  if (trimmed.empty())
    return std::string();  // Nothing to segment.

#if defined(OS_WIN)
  int trimmed_length = static_cast<int>(trimmed.length());
  if (url_parse::DoesBeginWindowsDriveSpec(trimmed.data(), 0, trimmed_length) ||
      url_parse::DoesBeginUNCPath(trimmed.data(), 0, trimmed_length, true))
    return "file";
#elif defined(OS_POSIX)
  if (FilePath::IsSeparator(trimmed.data()[0]) || trimmed.data()[0] == '~')
    return "file";
#endif

  std::string scheme;
  if (!GetValidScheme(text, &parts->scheme, &scheme)) {
    parts->scheme.reset();
    scheme.assign(StartsWithASCII(text, "ftp.", false) ?
        chrome::kFtpScheme : chrome::kHttpScheme);
  }

  if ((scheme != chrome::kAboutScheme) && (scheme != chrome::kChromeUIScheme) &&
      ((scheme == chrome::kFileScheme) || !url_util::IsStandard(scheme.c_str(),
           url_parse::Component(0, static_cast<int>(scheme.length())))))
    return scheme;

  if (parts->scheme.is_valid()) {
    url_parse::ParseStandardURL(text.data(), static_cast<int>(text.length()),
                                parts);
    return scheme;
  }

  std::string::const_iterator first_nonwhite = text.begin();
  while ((first_nonwhite != text.end()) && IsWhitespace(*first_nonwhite))
    ++first_nonwhite;

  std::string inserted_text(scheme);
  inserted_text.append(chrome::kStandardSchemeSeparator);
  std::string text_to_parse(text.begin(), first_nonwhite);
  text_to_parse.append(inserted_text);
  text_to_parse.append(first_nonwhite, text.end());

  url_parse::ParseStandardURL(text_to_parse.data(),
                              static_cast<int>(text_to_parse.length()),
                              parts);

  const int offset = -static_cast<int>(inserted_text.length());
  OffsetComponent(offset, &parts->scheme);
  OffsetComponent(offset, &parts->username);
  OffsetComponent(offset, &parts->password);
  OffsetComponent(offset, &parts->host);
  OffsetComponent(offset, &parts->port);
  OffsetComponent(offset, &parts->path);
  OffsetComponent(offset, &parts->query);
  OffsetComponent(offset, &parts->ref);

  return scheme;
}
