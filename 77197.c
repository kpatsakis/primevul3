bool Clipboard::IsFormatAvailableByString(const std::string& format,
                                          Clipboard::Buffer buffer) const {
  return IsFormatAvailable(format, buffer);
}
