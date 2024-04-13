int mz_uncompress(unsigned char *pDest, mz_ulong *pDest_len,
                  const unsigned char *pSource, mz_ulong source_len) {
  mz_stream stream;
  int status;
  memset(&stream, 0, sizeof(stream));

  // In case mz_ulong is 64-bits (argh I hate longs).
  if ((source_len | *pDest_len) > 0xFFFFFFFFU) return MZ_PARAM_ERROR;

  stream.next_in = pSource;
  stream.avail_in = (mz_uint32)source_len;
  stream.next_out = pDest;
  stream.avail_out = (mz_uint32)*pDest_len;

  status = mz_inflateInit(&stream);
  if (status != MZ_OK) return status;

  status = mz_inflate(&stream, MZ_FINISH);
  if (status != MZ_STREAM_END) {
    mz_inflateEnd(&stream);
    return ((status == MZ_BUF_ERROR) && (!stream.avail_in)) ? MZ_DATA_ERROR
                                                            : status;
  }
  *pDest_len = stream.total_out;

  return mz_inflateEnd(&stream);
}