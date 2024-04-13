GURL URLFixerUpper::FixupURL(const std::string& text,
                             const std::string& desired_tld) {
  std::string trimmed;
  TrimWhitespaceUTF8(text, TRIM_ALL, &trimmed);
  if (trimmed.empty())
    return GURL();  // Nothing here.

  url_parse::Parsed parts;
  std::string scheme(SegmentURL(trimmed, &parts));

  if (scheme == chrome::kViewSourceScheme) {
    std::string view_source(chrome::kViewSourceScheme + std::string(":"));
    if (!StartsWithASCII(text, view_source + view_source, false)) {
      return GURL(chrome::kViewSourceScheme + std::string(":") +
          FixupURL(trimmed.substr(scheme.length() + 1),
                   desired_tld).possibly_invalid_spec());
    }
  }

  if (scheme == chrome::kFileScheme)
    return GURL(parts.scheme.is_valid() ? text : FixupPath(text));

  bool chrome_url = !LowerCaseEqualsASCII(trimmed, chrome::kAboutBlankURL) &&
      ((scheme == chrome::kAboutScheme) || (scheme == chrome::kChromeUIScheme));

  if (chrome_url || url_util::IsStandard(scheme.c_str(),
          url_parse::Component(0, static_cast<int>(scheme.length())))) {
    std::string url(chrome_url ? chrome::kChromeUIScheme : scheme);
    url.append(chrome::kStandardSchemeSeparator);

    if (parts.username.is_valid()) {
      FixupUsername(trimmed, parts.username, &url);
      FixupPassword(trimmed, parts.password, &url);
      url.append("@");
    }

    FixupHost(trimmed, parts.host, parts.scheme.is_valid(), desired_tld, &url);
    if (chrome_url && !parts.host.is_valid())
      url.append(chrome::kChromeUIDefaultHost);
    FixupPort(trimmed, parts.port, &url);
    FixupPath(trimmed, parts.path, &url);
    FixupQuery(trimmed, parts.query, &url);
    FixupRef(trimmed, parts.ref, &url);

    return GURL(url);
  }

  if (!parts.scheme.is_valid()) {
    std::string fixed_scheme(scheme);
    fixed_scheme.append(chrome::kStandardSchemeSeparator);
    trimmed.insert(0, fixed_scheme);
  }

  return GURL(trimmed);
}
