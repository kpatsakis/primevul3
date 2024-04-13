static bool hufDecode(const long long *hcode,  // i : encoding table
                      const HufDec *hdecod,    // i : decoding table
                      const char *in,          // i : compressed input buffer
                      int ni,                  // i : input size (in bits)
                      int rlc,                 // i : run-length code
                      int no,  // i : expected output size (in bytes)
                      unsigned short *out)  //  o: uncompressed output buffer
{
  long long c = 0;
  int lc = 0;
  unsigned short *outb = out;          // begin
  unsigned short *oe = out + no;       // end
  const char *ie = in + (ni + 7) / 8;  // input byte size

  //
  // Loop on input bytes
  //

  while (in < ie) {
    getChar(c, lc, in);

    //
    // Access decoding table
    //

    while (lc >= HUF_DECBITS) {
      const HufDec pl = hdecod[(c >> (lc - HUF_DECBITS)) & HUF_DECMASK];

      if (pl.len) {
        //
        // Get short code
        //

        lc -= pl.len;
        // std::cout << "lit = " << pl.lit << std::endl;
        // std::cout << "rlc = " << rlc << std::endl;
        // std::cout << "c = " << c << std::endl;
        // std::cout << "lc = " << lc << std::endl;
        // std::cout << "in = " << in << std::endl;
        // std::cout << "out = " << out << std::endl;
        // std::cout << "oe = " << oe << std::endl;
        if (!getCode(pl.lit, rlc, c, lc, in, ie, out, outb, oe)) {
          return false;
        }
      } else {
        if (!pl.p) {
          return false;
        }
        // invalidCode(); // wrong code

        //
        // Search long code
        //

        int j;

        for (j = 0; j < pl.lit; j++) {
          int l = hufLength(hcode[pl.p[j]]);

          while (lc < l && in < ie)  // get more bits
            getChar(c, lc, in);

          if (lc >= l) {
            if (hufCode(hcode[pl.p[j]]) ==
                ((c >> (lc - l)) & (((long long)(1) << l) - 1))) {
              //
              // Found : get long code
              //

              lc -= l;
              if (!getCode(pl.p[j], rlc, c, lc, in, ie, out, outb, oe)) {
                return false;
              }
              break;
            }
          }
        }

        if (j == pl.lit) {
          return false;
          // invalidCode(); // Not found
        }
      }
    }
  }

  //
  // Get remaining (short) codes
  //

  int i = (8 - ni) & 7;
  c >>= i;
  lc -= i;

  while (lc > 0) {
    const HufDec pl = hdecod[(c << (HUF_DECBITS - lc)) & HUF_DECMASK];

    if (pl.len) {
      lc -= pl.len;
      if (!getCode(pl.lit, rlc, c, lc, in, ie, out, outb, oe)) {
        return false;
      }
    } else {
      return false;
      // invalidCode(); // wrong (long) code
    }
  }

  if (out - outb != no) {
    return false;
  }
  // notEnoughData ();

  return true;
}