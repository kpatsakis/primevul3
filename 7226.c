mz_ulong mz_deflateBound(mz_streamp pStream, mz_ulong source_len) {
  (void)pStream;
  // This is really over conservative. (And lame, but it's actually pretty
  // tricky to compute a true upper bound given the way tdefl's blocking works.)
  return MZ_MAX(128 + (source_len * 110) / 100,
                128 + source_len + ((source_len / (31 * 1024)) + 1) * 5);
}