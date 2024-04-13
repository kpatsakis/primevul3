bool ResolveRelativeURL(const char* base_url,
                        const url_parse::Parsed& base_parsed,
                        bool base_is_file,
                        const base::char16* relative_url,
                        const url_parse::Component& relative_component,
                        CharsetConverter* query_converter,
                        CanonOutput* output,
                        url_parse::Parsed* out_parsed) {
  return DoResolveRelativeURL<base::char16>(
      base_url, base_parsed, base_is_file, relative_url,
      relative_component, query_converter, output, out_parsed);
}
