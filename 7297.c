static void hufCanonicalCodeTable(long long hcode[HUF_ENCSIZE]) {
  long long n[59];

  //
  // For each i from 0 through 58, count the
  // number of different codes of length i, and
  // store the count in n[i].
  //

  for (int i = 0; i <= 58; ++i) n[i] = 0;

  for (int i = 0; i < HUF_ENCSIZE; ++i) n[hcode[i]] += 1;

  //
  // For each i from 58 through 1, compute the
  // numerically lowest code with length i, and
  // store that code in n[i].
  //

  long long c = 0;

  for (int i = 58; i > 0; --i) {
    long long nc = ((c + n[i]) >> 1);
    n[i] = c;
    c = nc;
  }

  //
  // hcode[i] contains the length, l, of the
  // code for symbol i.  Assign the next available
  // code of length l to the symbol and store both
  // l and the code in hcode[i].
  //

  for (int i = 0; i < HUF_ENCSIZE; ++i) {
    int l = static_cast<int>(hcode[i]);

    if (l > 0) hcode[i] = l | (n[l]++ << 6);
  }
}