inline void outputCode(long long code, long long &c, int &lc, char *&out) {
  outputBits(hufLength(code), hufCode(code), c, lc, out);
}