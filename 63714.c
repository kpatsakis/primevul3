divide (mpn_t a, mpn_t b, mpn_t *q)
{
  /* Algorithm:
     First normalise a and b: a=[a[m-1],...,a[0]], b=[b[n-1],...,b[0]]
     with m>=0 and n>0 (in base beta = 2^GMP_LIMB_BITS).
     If m<n, then q:=0 and r:=a.
     If m>=n=1, perform a single-precision division:
       r:=0, j:=m,
       while j>0 do
         {Here (q[m-1]*beta^(m-1)+...+q[j]*beta^j) * b[0] + r*beta^j =
               = a[m-1]*beta^(m-1)+...+a[j]*beta^j und 0<=r<b[0]<beta}
         j:=j-1, r:=r*beta+a[j], q[j]:=floor(r/b[0]), r:=r-b[0]*q[j].
       Normalise [q[m-1],...,q[0]], yields q.
     If m>=n>1, perform a multiple-precision division:
       We have a/b < beta^(m-n+1).
       s:=intDsize-1-(highest bit in b[n-1]), 0<=s<intDsize.
       Shift a and b left by s bits, copying them. r:=a.
       r=[r[m],...,r[0]], b=[b[n-1],...,b[0]] with b[n-1]>=beta/2.
       For j=m-n,...,0: {Here 0 <= r < b*beta^(j+1).}
         Compute q* :
           q* := floor((r[j+n]*beta+r[j+n-1])/b[n-1]).
           In case of overflow (q* >= beta) set q* := beta-1.
           Compute c2 := ((r[j+n]*beta+r[j+n-1]) - q* * b[n-1])*beta + r[j+n-2]
           and c3 := b[n-2] * q*.
           {We have 0 <= c2 < 2*beta^2, even 0 <= c2 < beta^2 if no overflow
            occurred.  Furthermore 0 <= c3 < beta^2.
            If there was overflow and
            r[j+n]*beta+r[j+n-1] - q* * b[n-1] >= beta, i.e. c2 >= beta^2,
            the next test can be skipped.}
           While c3 > c2, {Here 0 <= c2 < c3 < beta^2}
             Put q* := q* - 1, c2 := c2 + b[n-1]*beta, c3 := c3 - b[n-2].
           If q* > 0:
             Put r := r - b * q* * beta^j. In detail:
               [r[n+j],...,r[j]] := [r[n+j],...,r[j]] - q* * [b[n-1],...,b[0]].
               hence: u:=0, for i:=0 to n-1 do
                              u := u + q* * b[i],
                              r[j+i]:=r[j+i]-(u mod beta) (+ beta, if carry),
                              u:=u div beta (+ 1, if carry in subtraction)
                      r[n+j]:=r[n+j]-u.
               {Since always u = (q* * [b[i-1],...,b[0]] div beta^i) + 1
                               < q* + 1 <= beta,
                the carry u does not overflow.}
             If a negative carry occurs, put q* := q* - 1
               and [r[n+j],...,r[j]] := [r[n+j],...,r[j]] + [0,b[n-1],...,b[0]].
         Set q[j] := q*.
       Normalise [q[m-n],..,q[0]]; this yields the quotient q.
       Shift [r[n-1],...,r[0]] right by s bits and normalise; this yields the
       rest r.
       The room for q[j] can be allocated at the memory location of r[n+j].
     Finally, round-to-even:
       Shift r left by 1 bit.
       If r > b or if r = b and q[0] is odd, q := q+1.
   */
  const mp_limb_t *a_ptr = a.limbs;
  size_t a_len = a.nlimbs;
  const mp_limb_t *b_ptr = b.limbs;
  size_t b_len = b.nlimbs;
  mp_limb_t *roomptr;
  mp_limb_t *tmp_roomptr = NULL;
  mp_limb_t *q_ptr;
  size_t q_len;
  mp_limb_t *r_ptr;
  size_t r_len;

  /* Allocate room for a_len+2 digits.
     (Need a_len+1 digits for the real division and 1 more digit for the
     final rounding of q.)  */
  roomptr = (mp_limb_t *) malloc ((a_len + 2) * sizeof (mp_limb_t));
  if (roomptr == NULL)
    return NULL;

  /* Normalise a.  */
  while (a_len > 0 && a_ptr[a_len - 1] == 0)
    a_len--;

  /* Normalise b.  */
  for (;;)
    {
      if (b_len == 0)
        /* Division by zero.  */
        abort ();
      if (b_ptr[b_len - 1] == 0)
        b_len--;
      else
        break;
    }

  /* Here m = a_len >= 0 and n = b_len > 0.  */

  if (a_len < b_len)
    {
      /* m<n: trivial case.  q=0, r := copy of a.  */
      r_ptr = roomptr;
      r_len = a_len;
      memcpy (r_ptr, a_ptr, a_len * sizeof (mp_limb_t));
      q_ptr = roomptr + a_len;
      q_len = 0;
    }
  else if (b_len == 1)
    {
      /* n=1: single precision division.
         beta^(m-1) <= a < beta^m  ==>  beta^(m-2) <= a/b < beta^m  */
      r_ptr = roomptr;
      q_ptr = roomptr + 1;
      {
        mp_limb_t den = b_ptr[0];
        mp_limb_t remainder = 0;
        const mp_limb_t *sourceptr = a_ptr + a_len;
        mp_limb_t *destptr = q_ptr + a_len;
        size_t count;
        for (count = a_len; count > 0; count--)
          {
            mp_twolimb_t num =
              ((mp_twolimb_t) remainder << GMP_LIMB_BITS) | *--sourceptr;
            *--destptr = num / den;
            remainder = num % den;
          }
        /* Normalise and store r.  */
        if (remainder > 0)
          {
            r_ptr[0] = remainder;
            r_len = 1;
          }
        else
          r_len = 0;
        /* Normalise q.  */
        q_len = a_len;
        if (q_ptr[q_len - 1] == 0)
          q_len--;
      }
    }
  else
    {
      /* n>1: multiple precision division.
         beta^(m-1) <= a < beta^m, beta^(n-1) <= b < beta^n  ==>
         beta^(m-n-1) <= a/b < beta^(m-n+1).  */
      /* Determine s.  */
      size_t s;
      {
        mp_limb_t msd = b_ptr[b_len - 1]; /* = b[n-1], > 0 */
        /* Determine s = GMP_LIMB_BITS - integer_length (msd).
           Code copied from gnulib's integer_length.c.  */
# if __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 4)
        s = __builtin_clz (msd);
# else
#  if defined DBL_EXPBIT0_WORD && defined DBL_EXPBIT0_BIT
        if (GMP_LIMB_BITS <= DBL_MANT_BIT)
          {
            /* Use 'double' operations.
               Assumes an IEEE 754 'double' implementation.  */
#   define DBL_EXP_MASK ((DBL_MAX_EXP - DBL_MIN_EXP) | 7)
#   define DBL_EXP_BIAS (DBL_EXP_MASK / 2 - 1)
#   define NWORDS \
     ((sizeof (double) + sizeof (unsigned int) - 1) / sizeof (unsigned int))
            union { double value; unsigned int word[NWORDS]; } m;

            /* Use a single integer to floating-point conversion.  */
            m.value = msd;

            s = GMP_LIMB_BITS
                - (((m.word[DBL_EXPBIT0_WORD] >> DBL_EXPBIT0_BIT) & DBL_EXP_MASK)
                   - DBL_EXP_BIAS);
          }
        else
#   undef NWORDS
#  endif
          {
            s = 31;
            if (msd >= 0x10000)
              {
                msd = msd >> 16;
                s -= 16;
              }
            if (msd >= 0x100)
              {
                msd = msd >> 8;
                s -= 8;
              }
            if (msd >= 0x10)
              {
                msd = msd >> 4;
                s -= 4;
              }
            if (msd >= 0x4)
              {
                msd = msd >> 2;
                s -= 2;
              }
            if (msd >= 0x2)
              {
                msd = msd >> 1;
                s -= 1;
              }
          }
# endif
      }
      /* 0 <= s < GMP_LIMB_BITS.
         Copy b, shifting it left by s bits.  */
      if (s > 0)
        {
          tmp_roomptr = (mp_limb_t *) malloc (b_len * sizeof (mp_limb_t));
          if (tmp_roomptr == NULL)
            {
              free (roomptr);
              return NULL;
            }
          {
            const mp_limb_t *sourceptr = b_ptr;
            mp_limb_t *destptr = tmp_roomptr;
            mp_twolimb_t accu = 0;
            size_t count;
            for (count = b_len; count > 0; count--)
              {
                accu += (mp_twolimb_t) *sourceptr++ << s;
                *destptr++ = (mp_limb_t) accu;
                accu = accu >> GMP_LIMB_BITS;
              }
            /* accu must be zero, since that was how s was determined.  */
            if (accu != 0)
              abort ();
          }
          b_ptr = tmp_roomptr;
        }
      /* Copy a, shifting it left by s bits, yields r.
         Memory layout:
         At the beginning: r = roomptr[0..a_len],
         at the end: r = roomptr[0..b_len-1], q = roomptr[b_len..a_len]  */
      r_ptr = roomptr;
      if (s == 0)
        {
          memcpy (r_ptr, a_ptr, a_len * sizeof (mp_limb_t));
          r_ptr[a_len] = 0;
        }
      else
        {
          const mp_limb_t *sourceptr = a_ptr;
          mp_limb_t *destptr = r_ptr;
          mp_twolimb_t accu = 0;
          size_t count;
          for (count = a_len; count > 0; count--)
            {
              accu += (mp_twolimb_t) *sourceptr++ << s;
              *destptr++ = (mp_limb_t) accu;
              accu = accu >> GMP_LIMB_BITS;
            }
          *destptr++ = (mp_limb_t) accu;
        }
      q_ptr = roomptr + b_len;
      q_len = a_len - b_len + 1; /* q will have m-n+1 limbs */
      {
        size_t j = a_len - b_len; /* m-n */
        mp_limb_t b_msd = b_ptr[b_len - 1]; /* b[n-1] */
        mp_limb_t b_2msd = b_ptr[b_len - 2]; /* b[n-2] */
        mp_twolimb_t b_msdd = /* b[n-1]*beta+b[n-2] */
          ((mp_twolimb_t) b_msd << GMP_LIMB_BITS) | b_2msd;
        /* Division loop, traversed m-n+1 times.
           j counts down, b is unchanged, beta/2 <= b[n-1] < beta.  */
        for (;;)
          {
            mp_limb_t q_star;
            mp_limb_t c1;
            if (r_ptr[j + b_len] < b_msd) /* r[j+n] < b[n-1] ? */
              {
                /* Divide r[j+n]*beta+r[j+n-1] by b[n-1], no overflow.  */
                mp_twolimb_t num =
                  ((mp_twolimb_t) r_ptr[j + b_len] << GMP_LIMB_BITS)
                  | r_ptr[j + b_len - 1];
                q_star = num / b_msd;
                c1 = num % b_msd;
              }
            else
              {
                /* Overflow, hence r[j+n]*beta+r[j+n-1] >= beta*b[n-1].  */
                q_star = (mp_limb_t)~(mp_limb_t)0; /* q* = beta-1 */
                /* Test whether r[j+n]*beta+r[j+n-1] - (beta-1)*b[n-1] >= beta
                   <==> r[j+n]*beta+r[j+n-1] + b[n-1] >= beta*b[n-1]+beta
                   <==> b[n-1] < floor((r[j+n]*beta+r[j+n-1]+b[n-1])/beta)
                        {<= beta !}.
                   If yes, jump directly to the subtraction loop.
                   (Otherwise, r[j+n]*beta+r[j+n-1] - (beta-1)*b[n-1] < beta
                    <==> floor((r[j+n]*beta+r[j+n-1]+b[n-1])/beta) = b[n-1] ) */
                if (r_ptr[j + b_len] > b_msd
                    || (c1 = r_ptr[j + b_len - 1] + b_msd) < b_msd)
                  /* r[j+n] >= b[n-1]+1 or
                     r[j+n] = b[n-1] and the addition r[j+n-1]+b[n-1] gives a
                     carry.  */
                  goto subtract;
              }
            /* q_star = q*,
               c1 = (r[j+n]*beta+r[j+n-1]) - q* * b[n-1] (>=0, <beta).  */
            {
              mp_twolimb_t c2 = /* c1*beta+r[j+n-2] */
                ((mp_twolimb_t) c1 << GMP_LIMB_BITS) | r_ptr[j + b_len - 2];
              mp_twolimb_t c3 = /* b[n-2] * q* */
                (mp_twolimb_t) b_2msd * (mp_twolimb_t) q_star;
              /* While c2 < c3, increase c2 and decrease c3.
                 Consider c3-c2.  While it is > 0, decrease it by
                 b[n-1]*beta+b[n-2].  Because of b[n-1]*beta+b[n-2] >= beta^2/2
                 this can happen only twice.  */
              if (c3 > c2)
                {
                  q_star = q_star - 1; /* q* := q* - 1 */
                  if (c3 - c2 > b_msdd)
                    q_star = q_star - 1; /* q* := q* - 1 */
                }
            }
            if (q_star > 0)
              subtract:
              {
                /* Subtract r := r - b * q* * beta^j.  */
                mp_limb_t cr;
                {
                  const mp_limb_t *sourceptr = b_ptr;
                  mp_limb_t *destptr = r_ptr + j;
                  mp_twolimb_t carry = 0;
                  size_t count;
                  for (count = b_len; count > 0; count--)
                    {
                      /* Here 0 <= carry <= q*.  */
                      carry =
                        carry
                        + (mp_twolimb_t) q_star * (mp_twolimb_t) *sourceptr++
                        + (mp_limb_t) ~(*destptr);
                      /* Here 0 <= carry <= beta*q* + beta-1.  */
                      *destptr++ = ~(mp_limb_t) carry;
                      carry = carry >> GMP_LIMB_BITS; /* <= q* */
                    }
                  cr = (mp_limb_t) carry;
                }
                /* Subtract cr from r_ptr[j + b_len], then forget about
                   r_ptr[j + b_len].  */
                if (cr > r_ptr[j + b_len])
                  {
                    /* Subtraction gave a carry.  */
                    q_star = q_star - 1; /* q* := q* - 1 */
                    /* Add b back.  */
                    {
                      const mp_limb_t *sourceptr = b_ptr;
                      mp_limb_t *destptr = r_ptr + j;
                      mp_limb_t carry = 0;
                      size_t count;
                      for (count = b_len; count > 0; count--)
                        {
                          mp_limb_t source1 = *sourceptr++;
                          mp_limb_t source2 = *destptr;
                          *destptr++ = source1 + source2 + carry;
                          carry =
                            (carry
                             ? source1 >= (mp_limb_t) ~source2
                             : source1 > (mp_limb_t) ~source2);
                        }
                    }
                    /* Forget about the carry and about r[j+n].  */
                  }
              }
            /* q* is determined.  Store it as q[j].  */
            q_ptr[j] = q_star;
            if (j == 0)
              break;
            j--;
          }
      }
      r_len = b_len;
      /* Normalise q.  */
      if (q_ptr[q_len - 1] == 0)
        q_len--;
# if 0 /* Not needed here, since we need r only to compare it with b/2, and
          b is shifted left by s bits.  */
      /* Shift r right by s bits.  */
      if (s > 0)
        {
          mp_limb_t ptr = r_ptr + r_len;
          mp_twolimb_t accu = 0;
          size_t count;
          for (count = r_len; count > 0; count--)
            {
              accu = (mp_twolimb_t) (mp_limb_t) accu << GMP_LIMB_BITS;
              accu += (mp_twolimb_t) *--ptr << (GMP_LIMB_BITS - s);
              *ptr = (mp_limb_t) (accu >> GMP_LIMB_BITS);
            }
        }
# endif
      /* Normalise r.  */
      while (r_len > 0 && r_ptr[r_len - 1] == 0)
        r_len--;
    }
  /* Compare r << 1 with b.  */
  if (r_len > b_len)
    goto increment_q;
  {
    size_t i;
    for (i = b_len;;)
      {
        mp_limb_t r_i =
          (i <= r_len && i > 0 ? r_ptr[i - 1] >> (GMP_LIMB_BITS - 1) : 0)
          | (i < r_len ? r_ptr[i] << 1 : 0);
        mp_limb_t b_i = (i < b_len ? b_ptr[i] : 0);
        if (r_i > b_i)
          goto increment_q;
        if (r_i < b_i)
          goto keep_q;
        if (i == 0)
          break;
        i--;
      }
  }
  if (q_len > 0 && ((q_ptr[0] & 1) != 0))
    /* q is odd.  */
    increment_q:
    {
      size_t i;
      for (i = 0; i < q_len; i++)
        if (++(q_ptr[i]) != 0)
          goto keep_q;
      q_ptr[q_len++] = 1;
    }
  keep_q:
  if (tmp_roomptr != NULL)
    free (tmp_roomptr);
  q->limbs = q_ptr;
  q->nlimbs = q_len;
  return roomptr;
}
