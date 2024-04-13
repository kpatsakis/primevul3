std::string ProcessRawBits(const unsigned char* data, size_t data_length) {
  return ProcessRawBytes(data, (data_length + 7) / 8);
}
