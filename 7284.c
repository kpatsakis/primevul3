static void CompressZip(unsigned char *dst,
                        tinyexr::tinyexr_uint64 &compressedSize,
                        const unsigned char *src, unsigned long src_size) {
  std::vector<unsigned char> tmpBuf(src_size);

  //
  // Apply EXR-specific? postprocess. Grabbed from OpenEXR's
  // ImfZipCompressor.cpp
  //

  //
  // Reorder the pixel data.
  //

  const char *srcPtr = reinterpret_cast<const char *>(src);

  {
    char *t1 = reinterpret_cast<char *>(&tmpBuf.at(0));
    char *t2 = reinterpret_cast<char *>(&tmpBuf.at(0)) + (src_size + 1) / 2;
    const char *stop = srcPtr + src_size;

    for (;;) {
      if (srcPtr < stop)
        *(t1++) = *(srcPtr++);
      else
        break;

      if (srcPtr < stop)
        *(t2++) = *(srcPtr++);
      else
        break;
    }
  }

  //
  // Predictor.
  //

  {
    unsigned char *t = &tmpBuf.at(0) + 1;
    unsigned char *stop = &tmpBuf.at(0) + src_size;
    int p = t[-1];

    while (t < stop) {
      int d = int(t[0]) - p + (128 + 256);
      p = t[0];
      t[0] = static_cast<unsigned char>(d);
      ++t;
    }
  }

#if TINYEXR_USE_MINIZ
  //
  // Compress the data using miniz
  //

  miniz::mz_ulong outSize = miniz::mz_compressBound(src_size);
  int ret = miniz::mz_compress(
      dst, &outSize, static_cast<const unsigned char *>(&tmpBuf.at(0)),
      src_size);
  assert(ret == miniz::MZ_OK);
  (void)ret;

  compressedSize = outSize;
#else
  uLong outSize = compressBound(static_cast<uLong>(src_size));
  int ret = compress(dst, &outSize, static_cast<const Bytef *>(&tmpBuf.at(0)),
                     src_size);
  assert(ret == Z_OK);

  compressedSize = outSize;
#endif

  // Use uncompressed data when compressed data is larger than uncompressed.
  // (Issue 40)
  if (compressedSize >= src_size) {
    compressedSize = src_size;
    memcpy(dst, src, src_size);
  }
}