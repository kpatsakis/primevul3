int mz_deflate(mz_streamp pStream, int flush) {
  size_t in_bytes, out_bytes;
  mz_ulong orig_total_in, orig_total_out;
  int mz_status = MZ_OK;

  if ((!pStream) || (!pStream->state) || (flush < 0) || (flush > MZ_FINISH) ||
      (!pStream->next_out))
    return MZ_STREAM_ERROR;
  if (!pStream->avail_out) return MZ_BUF_ERROR;

  if (flush == MZ_PARTIAL_FLUSH) flush = MZ_SYNC_FLUSH;

  if (((tdefl_compressor *)pStream->state)->m_prev_return_status ==
      TDEFL_STATUS_DONE)
    return (flush == MZ_FINISH) ? MZ_STREAM_END : MZ_BUF_ERROR;

  orig_total_in = pStream->total_in;
  orig_total_out = pStream->total_out;
  for (;;) {
    tdefl_status defl_status;
    in_bytes = pStream->avail_in;
    out_bytes = pStream->avail_out;

    defl_status = tdefl_compress((tdefl_compressor *)pStream->state,
                                 pStream->next_in, &in_bytes, pStream->next_out,
                                 &out_bytes, (tdefl_flush)flush);
    pStream->next_in += (mz_uint)in_bytes;
    pStream->avail_in -= (mz_uint)in_bytes;
    pStream->total_in += (mz_uint)in_bytes;
    pStream->adler = tdefl_get_adler32((tdefl_compressor *)pStream->state);

    pStream->next_out += (mz_uint)out_bytes;
    pStream->avail_out -= (mz_uint)out_bytes;
    pStream->total_out += (mz_uint)out_bytes;

    if (defl_status < 0) {
      mz_status = MZ_STREAM_ERROR;
      break;
    } else if (defl_status == TDEFL_STATUS_DONE) {
      mz_status = MZ_STREAM_END;
      break;
    } else if (!pStream->avail_out)
      break;
    else if ((!pStream->avail_in) && (flush != MZ_FINISH)) {
      if ((flush) || (pStream->total_in != orig_total_in) ||
          (pStream->total_out != orig_total_out))
        break;
      return MZ_BUF_ERROR;  // Can't make forward progress without some input.
    }
  }
  return mz_status;
}