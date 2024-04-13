static bool GetValidScheme(const std::string &text,
                           url_parse::Component* scheme_component,
                           std::string* canon_scheme) {
  if (!url_parse::ExtractScheme(text.data(), static_cast<int>(text.length()),
                                scheme_component))
    return false;

  url_canon::StdStringCanonOutput canon_scheme_output(canon_scheme);
  url_parse::Component canon_scheme_component;
  if (!url_canon::CanonicalizeScheme(text.data(), *scheme_component,
                                     &canon_scheme_output,
                                     &canon_scheme_component))
    return false;

  DCHECK_EQ(0, canon_scheme_component.begin);
  canon_scheme->erase(canon_scheme_component.len);

  if (canon_scheme->find('.') != std::string::npos)
    return false;

  if (HasPort(text, *scheme_component))
    return false;

  return true;
}
