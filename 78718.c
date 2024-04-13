bool DoResolveRelativeURL(const char* base_url,
                          const url_parse::Parsed& base_parsed,
                          bool base_is_file,
                          const CHAR* relative_url,
                          const url_parse::Component& relative_component,
                          CharsetConverter* query_converter,
                          CanonOutput* output,
                          url_parse::Parsed* out_parsed) {
  *out_parsed = base_parsed;

  if (base_parsed.path.len <= 0) {
    int base_len = base_parsed.Length();
    for (int i = 0; i < base_len; i++)
      output->push_back(base_url[i]);
    return false;
  }

  if (relative_component.len <= 0) {
    int base_len = base_parsed.Length();
    base_len -= base_parsed.ref.len + 1;
    out_parsed->ref.reset();
    output->Append(base_url, base_len);
    return true;
  }

  int num_slashes = url_parse::CountConsecutiveSlashes(
      relative_url, relative_component.begin, relative_component.end());

#ifdef WIN32
  int after_slashes = relative_component.begin + num_slashes;
  if (url_parse::DoesBeginUNCPath(relative_url, relative_component.begin,
                                  relative_component.end(), !base_is_file) ||
      ((num_slashes == 0 || base_is_file) &&
       url_parse::DoesBeginWindowsDriveSpec(relative_url, after_slashes,
                                            relative_component.end()))) {
    return DoResolveAbsoluteFile(relative_url, relative_component,
                                 query_converter, output, out_parsed);
  }
#else
  if (base_is_file &&
      (num_slashes > 2 || num_slashes == relative_component.len)) {
    return DoResolveAbsoluteFile(relative_url, relative_component,
                                 query_converter, output, out_parsed);
  }
#endif

  if (num_slashes >= 2) {
    return DoResolveRelativeHost(base_url, base_parsed,
                                 relative_url, relative_component,
                                 query_converter, output, out_parsed);
  }

  return DoResolveRelativePath(base_url, base_parsed, base_is_file,
                               relative_url, relative_component,
                               query_converter, output, out_parsed);
}
