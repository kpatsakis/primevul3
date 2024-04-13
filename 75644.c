static void FixupHost(const std::string& text,
                      const url_parse::Component& part,
                      bool has_scheme,
                      const std::string& desired_tld,
                      std::string* url) {
  if (!part.is_valid())
    return;

  std::string domain(text, part.begin, part.len);
  const size_t first_nondot(domain.find_first_not_of('.'));
  if (first_nondot != std::string::npos) {
    domain.erase(0, first_nondot);
    size_t last_nondot(domain.find_last_not_of('.'));
    DCHECK(last_nondot != std::string::npos);
    last_nondot += 2;  // Point at second period in ending string
    if (last_nondot < domain.length())
      domain.erase(last_nondot);
  }

  AddDesiredTLD(desired_tld, &domain);

  url->append(domain);
}
