static void wav2Decode(
    unsigned short *in,  // io: values are transformed in place
    int nx,              // i : x size
    int ox,              // i : x offset
    int ny,              // i : y size
    int oy,              // i : y offset
    unsigned short mx)   // i : maximum in[x][y] value
{
  bool w14 = (mx < (1 << 14));
  int n = (nx > ny) ? ny : nx;
  int p = 1;
  int p2;

  //
  // Search max level
  //

  while (p <= n) p <<= 1;

  p >>= 1;
  p2 = p;
  p >>= 1;

  //
  // Hierarchical loop on smaller dimension n
  //

  while (p >= 1) {
    unsigned short *py = in;
    unsigned short *ey = in + oy * (ny - p2);
    int oy1 = oy * p;
    int oy2 = oy * p2;
    int ox1 = ox * p;
    int ox2 = ox * p2;
    unsigned short i00, i01, i10, i11;

    //
    // Y loop
    //

    for (; py <= ey; py += oy2) {
      unsigned short *px = py;
      unsigned short *ex = py + ox * (nx - p2);

      //
      // X loop
      //

      for (; px <= ex; px += ox2) {
        unsigned short *p01 = px + ox1;
        unsigned short *p10 = px + oy1;
        unsigned short *p11 = p10 + ox1;

        //
        // 2D wavelet decoding
        //

        if (w14) {
          wdec14(*px, *p10, i00, i10);
          wdec14(*p01, *p11, i01, i11);
          wdec14(i00, i01, *px, *p01);
          wdec14(i10, i11, *p10, *p11);
        } else {
          wdec16(*px, *p10, i00, i10);
          wdec16(*p01, *p11, i01, i11);
          wdec16(i00, i01, *px, *p01);
          wdec16(i10, i11, *p10, *p11);
        }
      }

      //
      // Decode (1D) odd column (still in Y loop)
      //

      if (nx & p) {
        unsigned short *p10 = px + oy1;

        if (w14)
          wdec14(*px, *p10, i00, *p10);
        else
          wdec16(*px, *p10, i00, *p10);

        *px = i00;
      }
    }

    //
    // Decode (1D) odd line (must loop in X)
    //

    if (ny & p) {
      unsigned short *px = py;
      unsigned short *ex = py + ox * (nx - p2);

      for (; px <= ex; px += ox2) {
        unsigned short *p01 = px + ox1;

        if (w14)
          wdec14(*px, *p01, i00, *p01);
        else
          wdec16(*px, *p01, i00, *p01);

        *px = i00;
      }
    }

    //
    // Next level
    //

    p2 = p;
    p >>= 1;
  }
}