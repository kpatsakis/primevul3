inline void wdec14(unsigned short l, unsigned short h, unsigned short &a,
                   unsigned short &b) {
  short ls = static_cast<short>(l);
  short hs = static_cast<short>(h);

  int hi = hs;
  int ai = ls + (hi & 1) + (hi >> 1);

  short as = static_cast<short>(ai);
  short bs = static_cast<short>(ai - hi);

  a = static_cast<unsigned short>(as);
  b = static_cast<unsigned short>(bs);
}