static int hufEncode            // return: output size (in bits)
    (const long long *hcode,    // i : encoding table
     const unsigned short *in,  // i : uncompressed input buffer
     const int ni,              // i : input buffer size (in bytes)
     int rlc,                   // i : rl code
     char *out)                 //  o: compressed output buffer
{
  char *outStart = out;
  long long c = 0;  // bits not yet written to out
  int lc = 0;       // number of valid bits in c (LSB)
  int s = in[0];
  int cs = 0;

  //
  // Loop on input values
  //

  for (int i = 1; i < ni; i++) {
    //
    // Count same values or send code
    //

    if (s == in[i] && cs < 255) {
      cs++;
    } else {
      sendCode(hcode[s], cs, hcode[rlc], c, lc, out);
      cs = 0;
    }

    s = in[i];
  }

  //
  // Send remaining code
  //

  sendCode(hcode[s], cs, hcode[rlc], c, lc, out);

  if (lc) *out = (c << (8 - lc)) & 0xff;

  return (out - outStart) * 8 + lc;
}