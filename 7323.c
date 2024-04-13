inline void wenc16(unsigned short a, unsigned short b, unsigned short &l,
                   unsigned short &h) {
  int ao = (a + A_OFFSET) & MOD_MASK;
  int m = ((ao + b) >> 1);
  int d = ao - b;

  if (d < 0) m = (m + M_OFFSET) & MOD_MASK;

  d &= MOD_MASK;

  l = static_cast<unsigned short>(m);
  h = static_cast<unsigned short>(d);
}