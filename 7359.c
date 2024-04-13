int LoadEXRImageFromMemory(EXRImage *exr_image, const EXRHeader *exr_header,
                           const unsigned char *memory, const size_t size,
                           const char **err) {
  if (exr_image == NULL || memory == NULL ||
      (size < tinyexr::kEXRVersionSize)) {
    tinyexr::SetErrorMessage("Invalid argument for LoadEXRImageFromMemory",
                             err);
    return TINYEXR_ERROR_INVALID_ARGUMENT;
  }

  if (exr_header->header_len == 0) {
    tinyexr::SetErrorMessage("EXRHeader variable is not initialized.", err);
    return TINYEXR_ERROR_INVALID_ARGUMENT;
  }

  const unsigned char *head = memory;
  const unsigned char *marker = reinterpret_cast<const unsigned char *>(
      memory + exr_header->header_len +
      8);  // +8 for magic number + version header.
  return tinyexr::DecodeEXRImage(exr_image, exr_header, head, marker, size,
                                 err);
}