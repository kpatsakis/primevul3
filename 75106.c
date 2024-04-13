void AppendFormattedHost(const GURL& url,
                         const std::wstring& languages,
                         std::wstring* output,
                         url_parse::Parsed* new_parsed,
                         size_t* offset_for_adjustment) {
  DCHECK(output);
  const url_parse::Component& host =
      url.parsed_for_possibly_invalid_spec().host;

  if (host.is_nonempty()) {
    int new_host_begin = static_cast<int>(output->length());
    if (new_parsed)
      new_parsed->host.begin = new_host_begin;
    size_t offset_past_current_output =
        (!offset_for_adjustment ||
         (*offset_for_adjustment == std::wstring::npos) ||
         (*offset_for_adjustment < output->length())) ?
            std::wstring::npos : (*offset_for_adjustment - output->length());
    size_t* offset_into_host =
        (offset_past_current_output >= static_cast<size_t>(host.len)) ?
            NULL : &offset_past_current_output;

    const std::string& spec = url.possibly_invalid_spec();
    DCHECK(host.begin >= 0 &&
           ((spec.length() == 0 && host.begin == 0) ||
            host.begin < static_cast<int>(spec.length())));
    output->append(net::IDNToUnicode(&spec[host.begin],
                   static_cast<size_t>(host.len), languages, offset_into_host));

    int new_host_len = static_cast<int>(output->length()) - new_host_begin;
    if (new_parsed)
      new_parsed->host.len = new_host_len;
    if (offset_into_host) {
      *offset_for_adjustment = (*offset_into_host == std::wstring::npos) ?
          std::wstring::npos : (new_host_begin + *offset_into_host);
    } else if (offset_past_current_output != std::wstring::npos) {
      *offset_for_adjustment += new_host_len - host.len;
    }
  } else if (new_parsed) {
    new_parsed->host.reset();
  }
}
