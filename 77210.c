void Clipboard::WriteBookmark(const char* title_data, size_t title_len,
                              const char* url_data, size_t url_len) {
  string16 url = UTF8ToUTF16(std::string(url_data, url_len) + "\n");
  string16 title = UTF8ToUTF16(std::string(title_data, title_len));
  int data_len = 2 * (title.length() + url.length());

  char* data = new char[data_len];
  memcpy(data, url.data(), 2 * url.length());
  memcpy(data + 2 * url.length(), title.data(), 2 * title.length());
  InsertMapping(kMimeTypeMozillaURL, data, data_len);
}
