multiply (mpn_t src1, mpn_t src2, mpn_t *dest)
{
  const mp_limb_t *p1;
  const mp_limb_t *p2;
  size_t len1;
  size_t len2;

  if (src1.nlimbs <= src2.nlimbs)
    {
      len1 = src1.nlimbs;
      p1 = src1.limbs;
      len2 = src2.nlimbs;
      p2 = src2.limbs;
    }
  else
    {
      len1 = src2.nlimbs;
      p1 = src2.limbs;
      len2 = src1.nlimbs;
      p2 = src1.limbs;
    }
  /* Now 0 <= len1 <= len2.  */
  if (len1 == 0)
    {
      /* src1 or src2 is zero.  */
      dest->nlimbs = 0;
      dest->limbs = (mp_limb_t *) malloc (1);
    }
  else
    {
      /* Here 1 <= len1 <= len2.  */
      size_t dlen;
      mp_limb_t *dp;
      size_t k, i, j;

      dlen = len1 + len2;
      dp = (mp_limb_t *) malloc (dlen * sizeof (mp_limb_t));
      if (dp == NULL)
        return NULL;
      for (k = len2; k > 0; )
        dp[--k] = 0;
      for (i = 0; i < len1; i++)
        {
          mp_limb_t digit1 = p1[i];
          mp_twolimb_t carry = 0;
          for (j = 0; j < len2; j++)
            {
              mp_limb_t digit2 = p2[j];
              carry += (mp_twolimb_t) digit1 * (mp_twolimb_t) digit2;
              carry += dp[i + j];
              dp[i + j] = (mp_limb_t) carry;
              carry = carry >> GMP_LIMB_BITS;
            }
          dp[i + len2] = (mp_limb_t) carry;
        }
      /* Normalise.  */
      while (dlen > 0 && dp[dlen - 1] == 0)
        dlen--;
      dest->nlimbs = dlen;
      dest->limbs = dp;
    }
  return dest->limbs;
}
