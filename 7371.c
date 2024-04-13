static MZ_FORCEINLINE void tdefl_find_match(
    tdefl_compressor *d, mz_uint lookahead_pos, mz_uint max_dist,
    mz_uint max_match_len, mz_uint *pMatch_dist, mz_uint *pMatch_len) {
  mz_uint dist, pos = lookahead_pos & TDEFL_LZ_DICT_SIZE_MASK,
                match_len = *pMatch_len, probe_pos = pos, next_probe_pos,
                probe_len;
  mz_uint num_probes_left = d->m_max_probes[match_len >= 32];
  const mz_uint16 *s = (const mz_uint16 *)(d->m_dict + pos), *p, *q;
  mz_uint16 c01 = TDEFL_READ_UNALIGNED_WORD(&d->m_dict[pos + match_len - 1]),
            s01 = TDEFL_READ_UNALIGNED_WORD(s);
  MZ_ASSERT(max_match_len <= TDEFL_MAX_MATCH_LEN);
  if (max_match_len <= match_len) return;
  for (;;) {
    for (;;) {
      if (--num_probes_left == 0) return;
#define TDEFL_PROBE                                                            \
  next_probe_pos = d->m_next[probe_pos];                                       \
  if ((!next_probe_pos) ||                                                     \
      ((dist = (mz_uint16)(lookahead_pos - next_probe_pos)) > max_dist))       \
    return;                                                                    \
  probe_pos = next_probe_pos & TDEFL_LZ_DICT_SIZE_MASK;                        \
  if (TDEFL_READ_UNALIGNED_WORD(&d->m_dict[probe_pos + match_len - 1]) == c01) \
    break;
      TDEFL_PROBE;
      TDEFL_PROBE;
      TDEFL_PROBE;
    }
    if (!dist) break;
    q = (const mz_uint16 *)(d->m_dict + probe_pos);
    if (TDEFL_READ_UNALIGNED_WORD(q) != s01) continue;
    p = s;
    probe_len = 32;
    do {
    } while (
        (TDEFL_READ_UNALIGNED_WORD(++p) == TDEFL_READ_UNALIGNED_WORD(++q)) &&
        (TDEFL_READ_UNALIGNED_WORD(++p) == TDEFL_READ_UNALIGNED_WORD(++q)) &&
        (TDEFL_READ_UNALIGNED_WORD(++p) == TDEFL_READ_UNALIGNED_WORD(++q)) &&
        (TDEFL_READ_UNALIGNED_WORD(++p) == TDEFL_READ_UNALIGNED_WORD(++q)) &&
        (--probe_len > 0));
    if (!probe_len) {
      *pMatch_dist = dist;
      *pMatch_len = MZ_MIN(max_match_len, TDEFL_MAX_MATCH_LEN);
      break;
    } else if ((probe_len = ((mz_uint)(p - s) * 2) +
                            (mz_uint)(*(const mz_uint8 *)p ==
                                      *(const mz_uint8 *)q)) > match_len) {
      *pMatch_dist = dist;
      if ((*pMatch_len = match_len = MZ_MIN(max_match_len, probe_len)) ==
          max_match_len)
        break;
      c01 = TDEFL_READ_UNALIGNED_WORD(&d->m_dict[pos + match_len - 1]);
    }
  }
}