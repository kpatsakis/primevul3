inline void wenc14(unsigned short a, unsigned short b, unsigned short &l,
                   unsigned short &h) {
  short as = static_cast<short>(a);
  short bs = static_cast<short>(b);

  short ms = (as + bs) >> 1;
  short ds = as - bs;

  l = static_cast<unsigned short>(ms);
  h = static_cast<unsigned short>(ds);
}