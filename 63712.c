decode_double (double x, int *ep, mpn_t *mp)
{
  mpn_t m;
  int exp;
  double y;
  size_t i;

  /* Allocate memory for result.  */
  m.nlimbs = (DBL_MANT_BIT + GMP_LIMB_BITS - 1) / GMP_LIMB_BITS;
  m.limbs = (mp_limb_t *) malloc (m.nlimbs * sizeof (mp_limb_t));
  if (m.limbs == NULL)
    return NULL;
  /* Split into exponential part and mantissa.  */
  y = frexp (x, &exp);
  if (!(y >= 0.0 && y < 1.0))
    abort ();
  /* x = 2^exp * y = 2^(exp - DBL_MANT_BIT) * (y * 2^DBL_MANT_BIT), and the
     latter is an integer.  */
  /* Convert the mantissa (y * 2^DBL_MANT_BIT) to a sequence of limbs.
     I'm not sure whether it's safe to cast a 'double' value between
     2^31 and 2^32 to 'unsigned int', therefore play safe and cast only
     'double' values between 0 and 2^16 (to 'unsigned int' or 'int',
     doesn't matter).  */
#  if (DBL_MANT_BIT % GMP_LIMB_BITS) != 0
#   if (DBL_MANT_BIT % GMP_LIMB_BITS) > GMP_LIMB_BITS / 2
    {
      mp_limb_t hi, lo;
      y *= (mp_limb_t) 1 << (DBL_MANT_BIT % (GMP_LIMB_BITS / 2));
      hi = (int) y;
      y -= hi;
      if (!(y >= 0.0 && y < 1.0))
        abort ();
      y *= (mp_limb_t) 1 << (GMP_LIMB_BITS / 2);
      lo = (int) y;
      y -= lo;
      if (!(y >= 0.0 && y < 1.0))
        abort ();
      m.limbs[DBL_MANT_BIT / GMP_LIMB_BITS] = (hi << (GMP_LIMB_BITS / 2)) | lo;
    }
#   else
    {
      mp_limb_t d;
      y *= (mp_limb_t) 1 << (DBL_MANT_BIT % GMP_LIMB_BITS);
      d = (int) y;
      y -= d;
      if (!(y >= 0.0 && y < 1.0))
        abort ();
      m.limbs[DBL_MANT_BIT / GMP_LIMB_BITS] = d;
    }
#   endif
#  endif
  for (i = DBL_MANT_BIT / GMP_LIMB_BITS; i > 0; )
    {
      mp_limb_t hi, lo;
      y *= (mp_limb_t) 1 << (GMP_LIMB_BITS / 2);
      hi = (int) y;
      y -= hi;
      if (!(y >= 0.0 && y < 1.0))
        abort ();
      y *= (mp_limb_t) 1 << (GMP_LIMB_BITS / 2);
      lo = (int) y;
      y -= lo;
      if (!(y >= 0.0 && y < 1.0))
        abort ();
      m.limbs[--i] = (hi << (GMP_LIMB_BITS / 2)) | lo;
    }
  if (!(y == 0.0))
    abort ();
  /* Normalise.  */
  while (m.nlimbs > 0 && m.limbs[m.nlimbs - 1] == 0)
    m.nlimbs--;
  *mp = m;
  *ep = exp - DBL_MANT_BIT;
  return m.limbs;
}
