void Clipboard::WriteHTML(const char* markup_data,
                          size_t markup_len,
                          const char* url_data,
                          size_t url_len) {
  static const char* html_prefix = "<meta http-equiv=\"content-type\" "
                                   "content=\"text/html; charset=utf-8\">";
  size_t html_prefix_len = strlen(html_prefix);
  size_t total_len = html_prefix_len + markup_len + 1;

  char* data = new char[total_len];
  snprintf(data, total_len, "%s", html_prefix);
  memcpy(data + html_prefix_len, markup_data, markup_len);
  data[total_len - 1] = '\0';

  InsertMapping(kMimeTypeHTML, data, total_len);
}
