FILE* OpenFile(const FilePath& filename, const char* mode) {
  base::ThreadRestrictions::AssertIOAllowed();
  FILE* result = NULL;
  do {
    result = fopen(filename.value().c_str(), mode);
  } while (!result && errno == EINTR);
  return result;
}
