inline void wdec16(unsigned short l, unsigned short h, unsigned short &a,
                   unsigned short &b) {
  int m = l;
  int d = h;
  int bb = (m - (d >> 1)) & MOD_MASK;
  int aa = (d + bb - A_OFFSET) & MOD_MASK;
  b = static_cast<unsigned short>(bb);
  a = static_cast<unsigned short>(aa);
}