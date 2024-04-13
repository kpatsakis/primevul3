std::wstring FormatUrl(const GURL& url,
                       const std::wstring& languages,
                       bool omit_username_password,
                       UnescapeRule::Type unescape_rules,
                       url_parse::Parsed* new_parsed,
                       size_t* prefix_end,
                       size_t* offset_for_adjustment) {
  url_parse::Parsed parsed_temp;
  if (!new_parsed)
    new_parsed = &parsed_temp;
  size_t offset_temp = std::wstring::npos;
  if (!offset_for_adjustment)
    offset_for_adjustment = &offset_temp;

  std::wstring url_string;

  if (url.is_empty()) {
    if (prefix_end)
      *prefix_end = 0;
    *offset_for_adjustment = std::wstring::npos;
    return url_string;
  }

  const char* const kViewSource = "view-source";
  const char* const kViewSourceTwice = "view-source:view-source:";
  if (url.SchemeIs(kViewSource) &&
      !StartsWithASCII(url.possibly_invalid_spec(), kViewSourceTwice, false)) {
    return FormatViewSourceUrl(url, languages, omit_username_password,
        unescape_rules, new_parsed, prefix_end, offset_for_adjustment);
  }

  const std::string& spec = url.possibly_invalid_spec();
  const url_parse::Parsed& parsed = url.parsed_for_possibly_invalid_spec();
  if (*offset_for_adjustment >= spec.length())
    *offset_for_adjustment = std::wstring::npos;

  std::copy(spec.begin(),
      spec.begin() + parsed.CountCharactersBefore(url_parse::Parsed::USERNAME,
                                                  true),
      std::back_inserter(url_string));
  new_parsed->scheme = parsed.scheme;

  if (omit_username_password) {
    new_parsed->username.reset();
    new_parsed->password.reset();
    if ((*offset_for_adjustment != std::wstring::npos) &&
        (parsed.username.is_nonempty() || parsed.password.is_nonempty())) {
      if (parsed.username.is_nonempty() && parsed.password.is_nonempty()) {
        if (*offset_for_adjustment >
            static_cast<size_t>(parsed.password.end())) {
          *offset_for_adjustment -=
              (parsed.username.len + parsed.password.len + 2);
        } else if (*offset_for_adjustment >
                   static_cast<size_t>(parsed.username.begin)) {
          *offset_for_adjustment = std::wstring::npos;
        }
      } else {
        const url_parse::Component* nonempty_component =
            parsed.username.is_nonempty() ? &parsed.username : &parsed.password;
        if (*offset_for_adjustment >
            static_cast<size_t>(nonempty_component->end())) {
          *offset_for_adjustment -= (nonempty_component->len + 1);
        } else if (*offset_for_adjustment >
                   static_cast<size_t>(nonempty_component->begin)) {
          *offset_for_adjustment = std::wstring::npos;
        }
      }
    }
  } else {
    AppendFormattedComponent(spec, parsed.username, unescape_rules, &url_string,
                             &new_parsed->username, offset_for_adjustment);
    if (parsed.password.is_valid()) {
      url_string.push_back(':');
    }
    AppendFormattedComponent(spec, parsed.password, unescape_rules, &url_string,
                             &new_parsed->password, offset_for_adjustment);
    if (parsed.username.is_valid() || parsed.password.is_valid()) {
      url_string.push_back('@');
    }
  }
  if (prefix_end)
    *prefix_end = static_cast<size_t>(url_string.length());

  AppendFormattedHost(url, languages, &url_string, new_parsed,
                      offset_for_adjustment);

  if (parsed.port.is_nonempty()) {
    url_string.push_back(':');
    new_parsed->port.begin = url_string.length();
    std::copy(spec.begin() + parsed.port.begin,
              spec.begin() + parsed.port.end(), std::back_inserter(url_string));
    new_parsed->port.len = url_string.length() - new_parsed->port.begin;
  } else {
    new_parsed->port.reset();
  }

  AppendFormattedComponent(spec, parsed.path, unescape_rules, &url_string,
                           &new_parsed->path, offset_for_adjustment);
  if (parsed.query.is_valid())
    url_string.push_back('?');
  AppendFormattedComponent(spec, parsed.query, unescape_rules, &url_string,
                           &new_parsed->query, offset_for_adjustment);

  if (parsed.ref.is_valid()) {
    url_string.push_back('#');
    new_parsed->ref.begin = url_string.length();
    size_t offset_past_current_output =
        ((*offset_for_adjustment == std::wstring::npos) ||
         (*offset_for_adjustment < url_string.length())) ?
            std::wstring::npos : (*offset_for_adjustment - url_string.length());
    size_t* offset_into_ref =
        (offset_past_current_output >= static_cast<size_t>(parsed.ref.len)) ?
            NULL : &offset_past_current_output;
    if (parsed.ref.len > 0) {
      url_string.append(UTF8ToWideAndAdjustOffset(spec.substr(parsed.ref.begin,
                                                              parsed.ref.len),
                                                  offset_into_ref));
    }
    new_parsed->ref.len = url_string.length() - new_parsed->ref.begin;
    if (offset_into_ref) {
      *offset_for_adjustment = (*offset_into_ref == std::wstring::npos) ?
          std::wstring::npos : (new_parsed->ref.begin + *offset_into_ref);
    } else if (offset_past_current_output != std::wstring::npos) {
      *offset_for_adjustment = std::wstring::npos;
    }
  }

  return url_string;
}
