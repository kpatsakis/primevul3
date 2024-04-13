static bool DecompressZip(unsigned char *dst,
                          unsigned long *uncompressed_size /* inout */,
                          const unsigned char *src, unsigned long src_size) {
  if ((*uncompressed_size) == src_size) {
    // Data is not compressed(Issue 40).
    memcpy(dst, src, src_size);
    return true;
  }
  std::vector<unsigned char> tmpBuf(*uncompressed_size);

#if TINYEXR_USE_MINIZ
  int ret =
      miniz::mz_uncompress(&tmpBuf.at(0), uncompressed_size, src, src_size);
  if (miniz::MZ_OK != ret) {
    return false;
  }
#else
  int ret = uncompress(&tmpBuf.at(0), uncompressed_size, src, src_size);
  if (Z_OK != ret) {
    return false;
  }
#endif

  //
  // Apply EXR-specific? postprocess. Grabbed from OpenEXR's
  // ImfZipCompressor.cpp
  //

  // Predictor.
  {
    unsigned char *t = &tmpBuf.at(0) + 1;
    unsigned char *stop = &tmpBuf.at(0) + (*uncompressed_size);

    while (t < stop) {
      int d = int(t[-1]) + int(t[0]) - 128;
      t[0] = static_cast<unsigned char>(d);
      ++t;
    }
  }

  // Reorder the pixel data.
  {
    const char *t1 = reinterpret_cast<const char *>(&tmpBuf.at(0));
    const char *t2 = reinterpret_cast<const char *>(&tmpBuf.at(0)) +
                     (*uncompressed_size + 1) / 2;
    char *s = reinterpret_cast<char *>(dst);
    char *stop = s + (*uncompressed_size);

    for (;;) {
      if (s < stop)
        *(s++) = *(t1++);
      else
        break;

      if (s < stop)
        *(s++) = *(t2++);
      else
        break;
    }
  }

  return true;
}