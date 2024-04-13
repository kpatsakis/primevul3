static inline void FixupUsername(const std::string& text,
                                 const url_parse::Component& part,
                                 std::string* url) {
  if (!part.is_valid())
    return;

  url->append(text, part.begin, part.len);
}
