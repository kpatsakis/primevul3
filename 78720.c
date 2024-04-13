bool IsRelativeURL(const char* base,
                   const url_parse::Parsed& base_parsed,
                   const base::char16* fragment,
                   int fragment_len,
                   bool is_base_hierarchical,
                   bool* is_relative,
                   url_parse::Component* relative_component) {
  return DoIsRelativeURL<base::char16>(
      base, base_parsed, fragment, fragment_len, is_base_hierarchical,
      is_relative, relative_component);
}
