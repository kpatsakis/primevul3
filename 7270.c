int ParseEXRHeaderFromMemory(EXRHeader *exr_header, const EXRVersion *version,
                             const unsigned char *memory, size_t size,
                             const char **err) {
  if (memory == NULL || exr_header == NULL) {
    tinyexr::SetErrorMessage(
        "Invalid argument. `memory` or `exr_header` argument is null in "
        "ParseEXRHeaderFromMemory()",
        err);

    // Invalid argument
    return TINYEXR_ERROR_INVALID_ARGUMENT;
  }

  if (size < tinyexr::kEXRVersionSize) {
    tinyexr::SetErrorMessage("Insufficient header/data size.\n", err);
    return TINYEXR_ERROR_INVALID_DATA;
  }

  const unsigned char *marker = memory + tinyexr::kEXRVersionSize;
  size_t marker_size = size - tinyexr::kEXRVersionSize;

  tinyexr::HeaderInfo info;
  info.clear();

  std::string err_str;
  int ret = ParseEXRHeader(&info, NULL, version, &err_str, marker, marker_size);

  if (ret != TINYEXR_SUCCESS) {
    if (err && !err_str.empty()) {
      tinyexr::SetErrorMessage(err_str, err);
    }
  }

  ConvertHeader(exr_header, info);

  // transfoer `tiled` from version.
  exr_header->tiled = version->tiled;

  return ret;
}