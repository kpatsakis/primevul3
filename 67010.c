NO_INLINE int getRadix(const char **s, int forceRadix, bool *hasError) {
  int radix = 10;

  if (forceRadix > 36) {
    if (hasError) *hasError = true;
    return 0;
  }

  if (**s == '0') {
    radix = 8;
    (*s)++;

    if (**s == 'o' || **s == 'O') {
      radix = 8;
      if (forceRadix && forceRadix!=8 && forceRadix<25) return 0;
      (*s)++;

    } else if (**s == 'x' || **s == 'X') {
      radix = 16;
      if (forceRadix && forceRadix!=16 && forceRadix<34) return 0;
      (*s)++;

    } else if (**s == 'b' || **s == 'B') {
      radix = 2;
      if (forceRadix && forceRadix!=2 && forceRadix<12)
        return 0;
      else
        (*s)++;
    } else if (!forceRadix) {
      const char *p;
      for (p=*s;*p;p++)
        if (*p=='.' || *p=='8' || *p=='9')
           radix = 10;
        else if (*p<'0' || *p>'9') break;
    }
  }
  if (forceRadix>0 && forceRadix<=36)
    radix = forceRadix;

  return radix;
}
