floorlog10l (long double x)
{
  int exp;
  long double y;
  double z;
  double l;

  /* Split into exponential part and mantissa.  */
  y = frexpl (x, &exp);
  if (!(y >= 0.0L && y < 1.0L))
    abort ();
  if (y == 0.0L)
    return INT_MIN;
  if (y < 0.5L)
    {
      while (y < (1.0L / (1 << (GMP_LIMB_BITS / 2)) / (1 << (GMP_LIMB_BITS / 2))))
        {
          y *= 1.0L * (1 << (GMP_LIMB_BITS / 2)) * (1 << (GMP_LIMB_BITS / 2));
          exp -= GMP_LIMB_BITS;
        }
      if (y < (1.0L / (1 << 16)))
        {
          y *= 1.0L * (1 << 16);
          exp -= 16;
        }
      if (y < (1.0L / (1 << 8)))
        {
          y *= 1.0L * (1 << 8);
          exp -= 8;
        }
      if (y < (1.0L / (1 << 4)))
        {
          y *= 1.0L * (1 << 4);
          exp -= 4;
        }
      if (y < (1.0L / (1 << 2)))
        {
          y *= 1.0L * (1 << 2);
          exp -= 2;
        }
      if (y < (1.0L / (1 << 1)))
        {
          y *= 1.0L * (1 << 1);
          exp -= 1;
        }
    }
  if (!(y >= 0.5L && y < 1.0L))
    abort ();
  /* Compute an approximation for l = log2(x) = exp + log2(y).  */
  l = exp;
  z = y;
  if (z < 0.70710678118654752444)
    {
      z *= 1.4142135623730950488;
      l -= 0.5;
    }
  if (z < 0.8408964152537145431)
    {
      z *= 1.1892071150027210667;
      l -= 0.25;
    }
  if (z < 0.91700404320467123175)
    {
      z *= 1.0905077326652576592;
      l -= 0.125;
    }
  if (z < 0.9576032806985736469)
    {
      z *= 1.0442737824274138403;
      l -= 0.0625;
    }
  /* Now 0.95 <= z <= 1.01.  */
  z = 1 - z;
  /* log2(1-z) = 1/log(2) * (- z - z^2/2 - z^3/3 - z^4/4 - ...)
     Four terms are enough to get an approximation with error < 10^-7.  */
  l -= 1.4426950408889634074 * z * (1.0 + z * (0.5 + z * ((1.0 / 3) + z * 0.25)));
  /* Finally multiply with log(2)/log(10), yields an approximation for
     log10(x).  */
  l *= 0.30102999566398119523;
  /* Round down to the next integer.  */
  return (int) l + (l < 0 ? -1 : 0);
}
