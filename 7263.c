int IsEXR(const char *filename) {
  EXRVersion exr_version;

  int ret = ParseEXRVersionFromFile(&exr_version, filename);
  if (ret != TINYEXR_SUCCESS) {
    return TINYEXR_ERROR_INVALID_HEADER;
  }

  return TINYEXR_SUCCESS;
}