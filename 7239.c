static void hufPackEncTable(
    const long long *hcode,  // i : encoding table [HUF_ENCSIZE]
    int im,                  // i : min hcode index
    int iM,                  // i : max hcode index
    char **pcode)            //  o: ptr to packed table (updated)
{
  char *p = *pcode;
  long long c = 0;
  int lc = 0;

  for (; im <= iM; im++) {
    int l = hufLength(hcode[im]);

    if (l == 0) {
      int zerun = 1;

      while ((im < iM) && (zerun < LONGEST_LONG_RUN)) {
        if (hufLength(hcode[im + 1]) > 0) break;
        im++;
        zerun++;
      }

      if (zerun >= 2) {
        if (zerun >= SHORTEST_LONG_RUN) {
          outputBits(6, LONG_ZEROCODE_RUN, c, lc, p);
          outputBits(8, zerun - SHORTEST_LONG_RUN, c, lc, p);
        } else {
          outputBits(6, SHORT_ZEROCODE_RUN + zerun - 2, c, lc, p);
        }
        continue;
      }
    }

    outputBits(6, l, c, lc, p);
  }

  if (lc > 0) *p++ = (unsigned char)(c << (8 - lc));

  *pcode = p;
}