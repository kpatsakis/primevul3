static bool DecompressRle(unsigned char *dst,
                          const unsigned long uncompressed_size,
                          const unsigned char *src, unsigned long src_size) {
  if (uncompressed_size == src_size) {
    // Data is not compressed(Issue 40).
    memcpy(dst, src, src_size);
    return true;
  }

  // Workaround for issue #112.
  // TODO(syoyo): Add more robust out-of-bounds check in `rleUncompress`.
  if (src_size <= 2) {
    return false;
  }

  std::vector<unsigned char> tmpBuf(uncompressed_size);

  int ret = rleUncompress(static_cast<int>(src_size),
                          static_cast<int>(uncompressed_size),
                          reinterpret_cast<const signed char *>(src),
                          reinterpret_cast<char *>(&tmpBuf.at(0)));
  if (ret != static_cast<int>(uncompressed_size)) {
    return false;
  }

  //
  // Apply EXR-specific? postprocess. Grabbed from OpenEXR's
  // ImfRleCompressor.cpp
  //

  // Predictor.
  {
    unsigned char *t = &tmpBuf.at(0) + 1;
    unsigned char *stop = &tmpBuf.at(0) + uncompressed_size;

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
                     (uncompressed_size + 1) / 2;
    char *s = reinterpret_cast<char *>(dst);
    char *stop = s + uncompressed_size;

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