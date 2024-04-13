static inline void FixupPath(const std::string& text,
                             const url_parse::Component& part,
                             std::string* url) {
  if (!part.is_valid() || part.len == 0) {
    url->append("/");
    return;
  }

  url->append(text, part.begin, part.len);
}
