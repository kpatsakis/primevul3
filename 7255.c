static bool getCode(int po, int rlc, long long &c, int &lc, const char *&in,
                    const char *in_end, unsigned short *&out,
                    const unsigned short *ob, const unsigned short *oe) {
  (void)ob;
  if (po == rlc) {
    if (lc < 8) {
      /* TinyEXR issue 78 */
      if ((in + 1) >= in_end) {
        return false;
      }

      getChar(c, lc, in);
    }

    lc -= 8;

    unsigned char cs = (c >> lc);

    if (out + cs > oe) return false;

    // Bounds check for safety
    // Issue 100.
    if ((out - 1) < ob) return false;
    unsigned short s = out[-1];

    while (cs-- > 0) *out++ = s;
  } else if (out < oe) {
    *out++ = po;
  } else {
    return false;
  }
  return true;
}