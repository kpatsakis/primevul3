bool DoResolveRelativePath(const char* base_url,
                           const url_parse::Parsed& base_parsed,
                           bool base_is_file,
                           const CHAR* relative_url,
                           const url_parse::Component& relative_component,
                           CharsetConverter* query_converter,
                           CanonOutput* output,
                           url_parse::Parsed* out_parsed) {
  bool success = true;

  url_parse::Component path, query, ref;
  url_parse::ParsePathInternal(relative_url,
                               relative_component,
                               &path,
                               &query,
                               &ref);
  output->Append(base_url, base_parsed.path.begin);

  if (path.len > 0) {
    int true_path_begin = output->length();

    int base_path_begin = base_parsed.path.begin;
#ifdef WIN32
    if (base_is_file) {
      base_path_begin = CopyBaseDriveSpecIfNecessary(
          base_url, base_parsed.path.begin, base_parsed.path.end(),
          relative_url, relative_component.begin, relative_component.end(),
          output);
    }
#endif  // WIN32

    if (url_parse::IsURLSlash(relative_url[path.begin])) {
      success &= CanonicalizePath(relative_url, path,
                                  output, &out_parsed->path);
    } else {
      int path_begin = output->length();
      CopyToLastSlash(base_url, base_path_begin, base_parsed.path.end(),
                      output);
      success &= CanonicalizePartialPath(relative_url, path, path_begin,
                                         output);
      out_parsed->path = url_parse::MakeRange(path_begin, output->length());

    }

    CanonicalizeQuery(relative_url, query, query_converter,
                      output, &out_parsed->query);
    CanonicalizeRef(relative_url, ref, output, &out_parsed->ref);

    out_parsed->path = url_parse::MakeRange(true_path_begin,
                                            out_parsed->path.end());
    return success;
  }

  CopyOneComponent(base_url, base_parsed.path, output, &out_parsed->path);

  if (query.is_valid()) {
    CanonicalizeQuery(relative_url, query, query_converter,
                      output, &out_parsed->query);
    CanonicalizeRef(relative_url, ref, output, &out_parsed->ref);
    return success;
  }

  if (base_parsed.query.is_valid())
    output->push_back('?');
  CopyOneComponent(base_url, base_parsed.query, output, &out_parsed->query);

  if (ref.is_valid()) {
    CanonicalizeRef(relative_url, ref, output, &out_parsed->ref);
    return success;
  }

  DCHECK(false) << "Not reached";
  return success;
}
