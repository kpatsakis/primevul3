int mz_inflateInit(mz_streamp pStream) {
  return mz_inflateInit2(pStream, MZ_DEFAULT_WINDOW_BITS);
}