static unsigned short forwardLutFromBitmap(
    const unsigned char bitmap[BITMAP_SIZE], unsigned short lut[USHORT_RANGE]) {
  int k = 0;

  for (int i = 0; i < USHORT_RANGE; ++i) {
    if ((i == 0) || (bitmap[i >> 3] & (1 << (i & 7))))
      lut[i] = k++;
    else
      lut[i] = 0;
  }

  return k - 1;  // maximum value stored in lut[],
}  // i.e. number of ones in bitmap minus 1