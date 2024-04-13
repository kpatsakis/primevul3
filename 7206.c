inline long long getBits(int nBits, long long &c, int &lc, const char *&in) {
  while (lc < nBits) {
    c = (c << 8) | *(reinterpret_cast<const unsigned char *>(in++));
    lc += 8;
  }

  lc -= nBits;
  return (c >> lc) & ((1 << nBits) - 1);
}