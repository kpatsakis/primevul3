decode_long_double (long double x, int *ep, mpn_t *mp)
{
  mpn_t m;
  int exp;
  long double y;
  size_t i;

  /* Allocate memory for result.  */
  m.nlimbs = (LDBL_MANT_BIT + GMP_LIMB_BITS - 1) / GMP_LIMB_BITS;
  m.limbs = (mp_limb_t *) malloc (m.nlimbs * sizeof (mp_limb_t));
  if (m.limbs == NULL)
    return NULL;
  /* Split into exponential part and mantissa.  */
  y = frexpl (x, &exp);
  if (!(y >= 0.0L && y < 1.0L))
    abort ();
  /* x = 2^exp * y = 2^(exp - LDBL_MANT_BIT) * (y * 2^LDBL_MANT_BIT), and the
     latter is an integer.  */
  /* Convert the mantissa (y * 2^LDBL_MANT_BIT) to a sequence of limbs.
     I'm not sure whether it's safe to cast a 'long double' value between
     2^31 and 2^32 to 'unsigned int', therefore play safe and cast only
     'long double' values between 0 and 2^16 (to 'unsigned int' or 'int',
     doesn't matter).  */
#  if (LDBL_MANT_BIT % GMP_LIMB_BITS) != 0
#   if (LDBL_MANT_BIT % GMP_LIMB_BITS) > GMP_LIMB_BITS / 2
    {
      mp_limb_t hi, lo;
      y *= (mp_limb_t) 1 << (LDBL_MANT_BIT % (GMP_LIMB_BITS / 2));
      hi = (int) y;
      y -= hi;
      if (!(y >= 0.0L && y < 1.0L))
        abort ();
      y *= (mp_limb_t) 1 << (GMP_LIMB_BITS / 2);
      lo = (int) y;
      y -= lo;
      if (!(y >= 0.0L && y < 1.0L))
        abort ();
      m.limbs[LDBL_MANT_BIT / GMP_LIMB_BITS] = (hi << (GMP_LIMB_BITS / 2)) | lo;
    }
#   else
    {
      mp_limb_t d;
      y *= (mp_limb_t) 1 << (LDBL_MANT_BIT % GMP_LIMB_BITS);
      d = (int) y;
      y -= d;
      if (!(y >= 0.0L && y < 1.0L))
        abort ();
      m.limbs[LDBL_MANT_BIT / GMP_LIMB_BITS] = d;
    }
#   endif
#  endif
  for (i = LDBL_MANT_BIT / GMP_LIMB_BITS; i > 0; )
    {
      mp_limb_t hi, lo;
      y *= (mp_limb_t) 1 << (GMP_LIMB_BITS / 2);
      hi = (int) y;
      y -= hi;
      if (!(y >= 0.0L && y < 1.0L))
        abort ();
      y *= (mp_limb_t) 1 << (GMP_LIMB_BITS / 2);
      lo = (int) y;
      y -= lo;
      if (!(y >= 0.0L && y < 1.0L))
        abort ();
      m.limbs[--i] = (hi << (GMP_LIMB_BITS / 2)) | lo;
    }
#  if 0 /* On FreeBSD 6.1/x86, 'long double' numbers sometimes have excess
           precision.  */
  if (!(y == 0.0L))
    abort ();
#  endif
  /* Normalise.  */
  while (m.nlimbs > 0 && m.limbs[m.nlimbs - 1] == 0)
    m.nlimbs--;
  *mp = m;
  *ep = exp - LDBL_MANT_BIT;
  return m.limbs;
}
