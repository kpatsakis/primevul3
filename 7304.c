static bool hufUnpackEncTable(
    const char **pcode,  // io: ptr to packed table (updated)
    int ni,              // i : input size (in bytes)
    int im,              // i : min hcode index
    int iM,              // i : max hcode index
    long long *hcode)    //  o: encoding table [HUF_ENCSIZE]
{
  memset(hcode, 0, sizeof(long long) * HUF_ENCSIZE);

  const char *p = *pcode;
  long long c = 0;
  int lc = 0;

  for (; im <= iM; im++) {
    if (p - *pcode >= ni) {
      return false;
    }

    long long l = hcode[im] = getBits(6, c, lc, p);  // code length

    if (l == (long long)LONG_ZEROCODE_RUN) {
      if (p - *pcode > ni) {
        return false;
      }

      int zerun = getBits(8, c, lc, p) + SHORTEST_LONG_RUN;

      if (im + zerun > iM + 1) {
        return false;
      }

      while (zerun--) hcode[im++] = 0;

      im--;
    } else if (l >= (long long)SHORT_ZEROCODE_RUN) {
      int zerun = l - SHORT_ZEROCODE_RUN + 2;

      if (im + zerun > iM + 1) {
        return false;
      }

      while (zerun--) hcode[im++] = 0;

      im--;
    }
  }

  *pcode = const_cast<char *>(p);

  hufCanonicalCodeTable(hcode);

  return true;
}