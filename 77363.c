std::string ProcessRawBytes(const unsigned char* data, size_t data_length) {
  return ProcessRawBytesWithSeparators(data, data_length, ' ', '\n');
}
