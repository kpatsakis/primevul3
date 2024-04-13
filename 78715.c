bool DoIsRelativeURL(const char* base,
                     const url_parse::Parsed& base_parsed,
                     const CHAR* url,
                     int url_len,
                     bool is_base_hierarchical,
                     bool* is_relative,
                     url_parse::Component* relative_component) {
  *is_relative = false;  // So we can default later to not relative.

  int begin = 0;
  url_parse::TrimURL(url, &begin, &url_len);
  if (begin >= url_len) {
    *relative_component = url_parse::Component(begin, 0);
    *is_relative = true;
    return true;
  }

#ifdef WIN32
  if (url_parse::DoesBeginWindowsDriveSpec(url, begin, url_len) ||
      url_parse::DoesBeginUNCPath(url, begin, url_len, true))
    return true;
#endif  // WIN32

  url_parse::Component scheme;
  if (!url_parse::ExtractScheme(url, url_len, &scheme) || scheme.len == 0) {
    if (!is_base_hierarchical)
      return false;

    *relative_component = url_parse::MakeRange(begin, url_len);
    *is_relative = true;
    return true;
  }

  int scheme_end = scheme.end();
  for (int i = scheme.begin; i < scheme_end; i++) {
    if (!CanonicalSchemeChar(url[i])) {
      *relative_component = url_parse::MakeRange(begin, url_len);
      *is_relative = true;
      return true;
    }
  }

  if (!AreSchemesEqual(base, base_parsed.scheme, url, scheme))
    return true;

  if (!is_base_hierarchical)
    return true;

  int colon_offset = scheme.end();

  if (url_util::CompareSchemeComponent(url, scheme, "filesystem"))
    return true;

  int num_slashes = url_parse::CountConsecutiveSlashes(url, colon_offset + 1,
                                                       url_len);

  if (num_slashes == 0 || num_slashes == 1) {
    *is_relative = true;
    *relative_component = url_parse::MakeRange(colon_offset + 1, url_len);
    return true;
  }

  return true;
}
