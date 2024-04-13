static unsigned short reverseLutFromBitmap(
    const unsigned char bitmap[BITMAP_SIZE], unsigned short lut[USHORT_RANGE]) {
  int k = 0;

  for (int i = 0; i < USHORT_RANGE; ++i) {
    if ((i == 0) || (bitmap[i >> 3] & (1 << (i & 7)))) lut[k++] = i;
  }

  int n = k - 1;

  while (k < USHORT_RANGE) lut[k++] = 0;

  return n;  // maximum k where lut[k] is non-zero,
}  // i.e. number of ones in bitmap minus 1