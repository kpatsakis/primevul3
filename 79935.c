FILE* OpenFile(const std::string& filename, const char* mode) {
  return OpenFile(FilePath(filename), mode);
}
