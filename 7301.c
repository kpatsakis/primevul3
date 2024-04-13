int mz_inflateEnd(mz_streamp pStream) {
  if (!pStream) return MZ_STREAM_ERROR;
  if (pStream->state) {
    pStream->zfree(pStream->opaque, pStream->state);
    pStream->state = NULL;
  }
  return MZ_OK;
}