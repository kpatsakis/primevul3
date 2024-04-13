void Clipboard::WriteText(const char* text_data, size_t text_len) {
  char* data = new char[text_len];
  memcpy(data, text_data, text_len);

  InsertMapping(kMimeTypeText, data, text_len);
  InsertMapping("TEXT", data, text_len);
  InsertMapping("STRING", data, text_len);
  InsertMapping("UTF8_STRING", data, text_len);
  InsertMapping("COMPOUND_TEXT", data, text_len);
}
