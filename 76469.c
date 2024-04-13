  ReadPixelsEmulator(GLsizei width, GLsizei height, GLint bytes_per_pixel,
                     const void* pixels, GLint pack_alignment)
      : width_(width),
        height_(height),
        pack_alignment_(pack_alignment),
        bytes_per_pixel_(bytes_per_pixel),
        pixels_(reinterpret_cast<const int8*>(pixels)) {
  }
