int LoadEXRImageFromFile(EXRImage *exr_image, const EXRHeader *exr_header,
                         const char *filename, const char **err) {
  if (exr_image == NULL) {
    tinyexr::SetErrorMessage("Invalid argument for LoadEXRImageFromFile", err);
    return TINYEXR_ERROR_INVALID_ARGUMENT;
  }

#ifdef _WIN32
  FILE *fp = NULL;
  fopen_s(&fp, filename, "rb");
#else
  FILE *fp = fopen(filename, "rb");
#endif
  if (!fp) {
    tinyexr::SetErrorMessage("Cannot read file " + std::string(filename), err);
    return TINYEXR_ERROR_CANT_OPEN_FILE;
  }

  size_t filesize;
  // Compute size
  fseek(fp, 0, SEEK_END);
  filesize = static_cast<size_t>(ftell(fp));
  fseek(fp, 0, SEEK_SET);

  if (filesize < 16) {
    tinyexr::SetErrorMessage("File size too short " + std::string(filename),
                             err);
    return TINYEXR_ERROR_INVALID_FILE;
  }

  std::vector<unsigned char> buf(filesize);  // @todo { use mmap }
  {
    size_t ret;
    ret = fread(&buf[0], 1, filesize, fp);
    assert(ret == filesize);
    fclose(fp);
    (void)ret;
  }

  return LoadEXRImageFromMemory(exr_image, exr_header, &buf.at(0), filesize,
                                err);
}