bool DoResolveAbsoluteFile(const CHAR* relative_url,
                           const url_parse::Component& relative_component,
                           CharsetConverter* query_converter,
                           CanonOutput* output,
                           url_parse::Parsed* out_parsed) {
  url_parse::Parsed relative_parsed;
  url_parse::ParseFileURL(&relative_url[relative_component.begin],
                          relative_component.len, &relative_parsed);

  return CanonicalizeFileURL(&relative_url[relative_component.begin],
                             relative_component.len, relative_parsed,
                             query_converter, output, out_parsed);
}
