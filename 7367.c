inline void outputBits(int nBits, long long bits, long long &c, int &lc,
                       char *&out) {
  c <<= nBits;
  lc += nBits;

  c |= bits;

  while (lc >= 8) *out++ = static_cast<char>((c >> (lc -= 8)));
}