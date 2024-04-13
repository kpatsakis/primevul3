bool AreSchemesEqual(const char* base,
                     const url_parse::Component& base_scheme,
                     const CHAR* cmp,
                     const url_parse::Component& cmp_scheme) {
  if (base_scheme.len != cmp_scheme.len)
    return false;
  for (int i = 0; i < base_scheme.len; i++) {
    if (CanonicalSchemeChar(cmp[cmp_scheme.begin + i]) !=
        base[base_scheme.begin + i])
      return false;
  }
  return true;
}
