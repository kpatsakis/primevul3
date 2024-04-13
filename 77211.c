void Clipboard::WriteData(const char* format_name, size_t format_len,
                          const char* data_data, size_t data_len) {
  std::string format(format_name, format_len);
  if (format == kMimeTypeBitmap)
    return;
  char* data = new char[data_len];
  memcpy(data, data_data, data_len);
  InsertMapping(format.c_str(), data, data_len);
}
