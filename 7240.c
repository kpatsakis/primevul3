static void wav2Encode(
    unsigned short *in,  // io: values are transformed in place
    int nx,              // i : x size
    int ox,              // i : x offset
    int ny,              // i : y size
    int oy,              // i : y offset
    unsigned short mx)   // i : maximum in[x][y] value
{
  bool w14 = (mx < (1 << 14));
  int n = (nx > ny) ? ny : nx;
  int p = 1;   // == 1 <<  level
  int p2 = 2;  // == 1 << (level+1)

  //
  // Hierachical loop on smaller dimension n
  //

  while (p2 <= n) {
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
        // 2D wavelet encoding
        //

        if (w14) {
          wenc14(*px, *p01, i00, i01);
          wenc14(*p10, *p11, i10, i11);
          wenc14(i00, i10, *px, *p10);
          wenc14(i01, i11, *p01, *p11);
        } else {
          wenc16(*px, *p01, i00, i01);
          wenc16(*p10, *p11, i10, i11);
          wenc16(i00, i10, *px, *p10);
          wenc16(i01, i11, *p01, *p11);
        }
      }

      //
      // Encode (1D) odd column (still in Y loop)
      //

      if (nx & p) {
        unsigned short *p10 = px + oy1;

        if (w14)
          wenc14(*px, *p10, i00, *p10);
        else
          wenc16(*px, *p10, i00, *p10);

        *px = i00;
      }
    }

    //
    // Encode (1D) odd line (must loop in X)
    //

    if (ny & p) {
      unsigned short *px = py;
      unsigned short *ex = py + ox * (nx - p2);

      for (; px <= ex; px += ox2) {
        unsigned short *p01 = px + ox1;

        if (w14)
          wenc14(*px, *p01, i00, *p01);
        else
          wenc16(*px, *p01, i00, *p01);

        *px = i00;
      }
    }

    //
    // Next level
    //

    p = p2;
    p2 <<= 1;
  }
}