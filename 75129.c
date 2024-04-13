bool IsCanonicalizedHostCompliant(const std::string& host) {
  if (host.empty())
    return false;

  bool in_component = false;
  bool most_recent_component_started_alpha = false;
  bool last_char_was_hyphen_or_underscore = false;

  for (std::string::const_iterator i(host.begin()); i != host.end(); ++i) {
    const char c = *i;
    if (!in_component) {
      most_recent_component_started_alpha = IsHostCharAlpha(c);
      if (!most_recent_component_started_alpha && !IsHostCharDigit(c))
        return false;
      in_component = true;
    } else {
      if (c == '.') {
        if (last_char_was_hyphen_or_underscore)
          return false;
        in_component = false;
      } else if (IsHostCharAlpha(c) || IsHostCharDigit(c)) {
        last_char_was_hyphen_or_underscore = false;
      } else if ((c == '-') || (c == '_')) {
        last_char_was_hyphen_or_underscore = true;
      } else {
        return false;
      }
    }
  }

  return most_recent_component_started_alpha;
}
