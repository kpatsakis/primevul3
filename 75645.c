static inline void FixupPassword(const std::string& text,
                                 const url_parse::Component& part,
                                 std::string* url) {
  if (!part.is_valid())
    return;

  url->append(":");
  url->append(text, part.begin, part.len);
}
