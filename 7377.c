int SaveEXRImageToFile(const EXRImage *exr_image, const EXRHeader *exr_header,
                       const char *filename, const char **err) {
  if (exr_image == NULL || filename == NULL ||
      exr_header->compression_type < 0) {
    tinyexr::SetErrorMessage("Invalid argument for SaveEXRImageToFile", err);
    return TINYEXR_ERROR_INVALID_ARGUMENT;
  }

#if !TINYEXR_USE_PIZ
  if (exr_header->compression_type == TINYEXR_COMPRESSIONTYPE_PIZ) {
    tinyexr::SetErrorMessage("PIZ compression is not supported in this build",
                             err);
    return TINYEXR_ERROR_UNSUPPORTED_FEATURE;
  }
#endif

#if !TINYEXR_USE_ZFP
  if (exr_header->compression_type == TINYEXR_COMPRESSIONTYPE_ZFP) {
    tinyexr::SetErrorMessage("ZFP compression is not supported in this build",
                             err);
    return TINYEXR_ERROR_UNSUPPORTED_FEATURE;
  }
#endif

#ifdef _WIN32
  FILE *fp = NULL;
  fopen_s(&fp, filename, "wb");
#else
  FILE *fp = fopen(filename, "wb");
#endif
  if (!fp) {
    tinyexr::SetErrorMessage("Cannot write a file", err);
    return TINYEXR_ERROR_CANT_WRITE_FILE;
  }

  unsigned char *mem = NULL;
  size_t mem_size = SaveEXRImageToMemory(exr_image, exr_header, &mem, err);
  if (mem_size == 0) {
    return TINYEXR_ERROR_SERIALZATION_FAILED;
  }

  size_t written_size = 0;
  if ((mem_size > 0) && mem) {
    written_size = fwrite(mem, 1, mem_size, fp);
  }
  free(mem);

  fclose(fp);

  if (written_size != mem_size) {
    tinyexr::SetErrorMessage("Cannot write a file", err);
    return TINYEXR_ERROR_CANT_WRITE_FILE;
  }

  return TINYEXR_SUCCESS;
}