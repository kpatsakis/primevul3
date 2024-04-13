void *tdefl_write_image_to_png_file_in_memory(const void *pImage, int w, int h,
                                              int num_chans, size_t *pLen_out) {
  // Level 6 corresponds to TDEFL_DEFAULT_MAX_PROBES or MZ_DEFAULT_LEVEL (but we
  // can't depend on MZ_DEFAULT_LEVEL being available in case the zlib API's
  // where #defined out)
  return tdefl_write_image_to_png_file_in_memory_ex(pImage, w, h, num_chans,
                                                    pLen_out, 6, MZ_FALSE);
}