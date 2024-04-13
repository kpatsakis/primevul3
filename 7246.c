static bool hufBuildDecTable(const long long *hcode,  // i : encoding table
                             int im,                  // i : min index in hcode
                             int iM,                  // i : max index in hcode
                             HufDec *hdecod)  //  o: (allocated by caller)
//     decoding table [HUF_DECSIZE]
{
  //
  // Init hashtable & loop on all codes.
  // Assumes that hufClearDecTable(hdecod) has already been called.
  //

  for (; im <= iM; im++) {
    long long c = hufCode(hcode[im]);
    int l = hufLength(hcode[im]);

    if (c >> l) {
      //
      // Error: c is supposed to be an l-bit code,
      // but c contains a value that is greater
      // than the largest l-bit number.
      //

      // invalidTableEntry();
      return false;
    }

    if (l > HUF_DECBITS) {
      //
      // Long code: add a secondary entry
      //

      HufDec *pl = hdecod + (c >> (l - HUF_DECBITS));

      if (pl->len) {
        //
        // Error: a short code has already
        // been stored in table entry *pl.
        //

        // invalidTableEntry();
        return false;
      }

      pl->lit++;

      if (pl->p) {
        int *p = pl->p;
        pl->p = new int[pl->lit];

        for (int i = 0; i < pl->lit - 1; ++i) pl->p[i] = p[i];

        delete[] p;
      } else {
        pl->p = new int[1];
      }

      pl->p[pl->lit - 1] = im;
    } else if (l) {
      //
      // Short code: init all primary entries
      //

      HufDec *pl = hdecod + (c << (HUF_DECBITS - l));

      for (long long i = 1ULL << (HUF_DECBITS - l); i > 0; i--, pl++) {
        if (pl->len || pl->p) {
          //
          // Error: a short code or a long code has
          // already been stored in table entry *pl.
          //

          // invalidTableEntry();
          return false;
        }

        pl->len = l;
        pl->lit = im;
      }
    }
  }

  return true;
}