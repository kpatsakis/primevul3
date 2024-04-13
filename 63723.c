scale10_round_decimal_decoded (int e, mpn_t m, void *memory, int n)
{
  int s;
  size_t extra_zeroes;
  unsigned int abs_n;
  unsigned int abs_s;
  mp_limb_t *pow5_ptr;
  size_t pow5_len;
  unsigned int s_limbs;
  unsigned int s_bits;
  mpn_t pow5;
  mpn_t z;
  void *z_memory;
  char *digits;

  if (memory == NULL)
    return NULL;
  /* x = 2^e * m, hence
     y = round (2^e * 10^n * m) = round (2^(e+n) * 5^n * m)
       = round (2^s * 5^n * m).  */
  s = e + n;
  extra_zeroes = 0;
  /* Factor out a common power of 10 if possible.  */
  if (s > 0 && n > 0)
    {
      extra_zeroes = (s < n ? s : n);
      s -= extra_zeroes;
      n -= extra_zeroes;
    }
  /* Here y = round (2^s * 5^n * m) * 10^extra_zeroes.
     Before converting to decimal, we need to compute
     z = round (2^s * 5^n * m).  */
  /* Compute 5^|n|, possibly shifted by |s| bits if n and s have the same
     sign.  2.322 is slightly larger than log(5)/log(2).  */
  abs_n = (n >= 0 ? n : -n);
  abs_s = (s >= 0 ? s : -s);
  pow5_ptr = (mp_limb_t *) malloc (((int)(abs_n * (2.322f / GMP_LIMB_BITS)) + 1
                                    + abs_s / GMP_LIMB_BITS + 1)
                                   * sizeof (mp_limb_t));
  if (pow5_ptr == NULL)
    {
      free (memory);
      return NULL;
    }
  /* Initialize with 1.  */
  pow5_ptr[0] = 1;
  pow5_len = 1;
  /* Multiply with 5^|n|.  */
  if (abs_n > 0)
    {
      static mp_limb_t const small_pow5[13 + 1] =
        {
          1, 5, 25, 125, 625, 3125, 15625, 78125, 390625, 1953125, 9765625,
          48828125, 244140625, 1220703125
        };
      unsigned int n13;
      for (n13 = 0; n13 <= abs_n; n13 += 13)
        {
          mp_limb_t digit1 = small_pow5[n13 + 13 <= abs_n ? 13 : abs_n - n13];
          size_t j;
          mp_twolimb_t carry = 0;
          for (j = 0; j < pow5_len; j++)
            {
              mp_limb_t digit2 = pow5_ptr[j];
              carry += (mp_twolimb_t) digit1 * (mp_twolimb_t) digit2;
              pow5_ptr[j] = (mp_limb_t) carry;
              carry = carry >> GMP_LIMB_BITS;
            }
          if (carry > 0)
            pow5_ptr[pow5_len++] = (mp_limb_t) carry;
        }
    }
  s_limbs = abs_s / GMP_LIMB_BITS;
  s_bits = abs_s % GMP_LIMB_BITS;
  if (n >= 0 ? s >= 0 : s <= 0)
    {
      /* Multiply with 2^|s|.  */
      if (s_bits > 0)
        {
          mp_limb_t *ptr = pow5_ptr;
          mp_twolimb_t accu = 0;
          size_t count;
          for (count = pow5_len; count > 0; count--)
            {
              accu += (mp_twolimb_t) *ptr << s_bits;
              *ptr++ = (mp_limb_t) accu;
              accu = accu >> GMP_LIMB_BITS;
            }
          if (accu > 0)
            {
              *ptr = (mp_limb_t) accu;
              pow5_len++;
            }
        }
      if (s_limbs > 0)
        {
          size_t count;
          for (count = pow5_len; count > 0;)
            {
              count--;
              pow5_ptr[s_limbs + count] = pow5_ptr[count];
            }
          for (count = s_limbs; count > 0;)
            {
              count--;
              pow5_ptr[count] = 0;
            }
          pow5_len += s_limbs;
        }
      pow5.limbs = pow5_ptr;
      pow5.nlimbs = pow5_len;
      if (n >= 0)
        {
          /* Multiply m with pow5.  No division needed.  */
          z_memory = multiply (m, pow5, &z);
        }
      else
        {
          /* Divide m by pow5 and round.  */
          z_memory = divide (m, pow5, &z);
        }
    }
  else
    {
      pow5.limbs = pow5_ptr;
      pow5.nlimbs = pow5_len;
      if (n >= 0)
        {
          /* n >= 0, s < 0.
             Multiply m with pow5, then divide by 2^|s|.  */
          mpn_t numerator;
          mpn_t denominator;
          void *tmp_memory;
          tmp_memory = multiply (m, pow5, &numerator);
          if (tmp_memory == NULL)
            {
              free (pow5_ptr);
              free (memory);
              return NULL;
            }
          /* Construct 2^|s|.  */
          {
            mp_limb_t *ptr = pow5_ptr + pow5_len;
            size_t i;
            for (i = 0; i < s_limbs; i++)
              ptr[i] = 0;
            ptr[s_limbs] = (mp_limb_t) 1 << s_bits;
            denominator.limbs = ptr;
            denominator.nlimbs = s_limbs + 1;
          }
          z_memory = divide (numerator, denominator, &z);
          free (tmp_memory);
        }
      else
        {
          /* n < 0, s > 0.
             Multiply m with 2^s, then divide by pow5.  */
          mpn_t numerator;
          mp_limb_t *num_ptr;
          num_ptr = (mp_limb_t *) malloc ((m.nlimbs + s_limbs + 1)
                                          * sizeof (mp_limb_t));
          if (num_ptr == NULL)
            {
              free (pow5_ptr);
              free (memory);
              return NULL;
            }
          {
            mp_limb_t *destptr = num_ptr;
            {
              size_t i;
              for (i = 0; i < s_limbs; i++)
                *destptr++ = 0;
            }
            if (s_bits > 0)
              {
                const mp_limb_t *sourceptr = m.limbs;
                mp_twolimb_t accu = 0;
                size_t count;
                for (count = m.nlimbs; count > 0; count--)
                  {
                    accu += (mp_twolimb_t) *sourceptr++ << s_bits;
                    *destptr++ = (mp_limb_t) accu;
                    accu = accu >> GMP_LIMB_BITS;
                  }
                if (accu > 0)
                  *destptr++ = (mp_limb_t) accu;
              }
            else
              {
                const mp_limb_t *sourceptr = m.limbs;
                size_t count;
                for (count = m.nlimbs; count > 0; count--)
                  *destptr++ = *sourceptr++;
              }
            numerator.limbs = num_ptr;
            numerator.nlimbs = destptr - num_ptr;
          }
          z_memory = divide (numerator, pow5, &z);
          free (num_ptr);
        }
    }
  free (pow5_ptr);
  free (memory);

  /* Here y = round (x * 10^n) = z * 10^extra_zeroes.  */

  if (z_memory == NULL)
    return NULL;
  digits = convert_to_decimal (z, extra_zeroes);
  free (z_memory);
  return digits;
}
