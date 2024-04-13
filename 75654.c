static bool HasPort(const std::string& original_text,
                    const url_parse::Component& scheme_component) {
  size_t port_start = scheme_component.end() + 1;
  size_t port_end = port_start;
  while ((port_end < original_text.length()) &&
         !url_parse::IsAuthorityTerminator(original_text[port_end]))
    ++port_end;
  if (port_end == port_start)
    return false;

  for (size_t i = port_start; i < port_end; ++i) {
    if (!IsAsciiDigit(original_text[i]))
      return false;
  }

  return true;
}
