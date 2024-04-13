long long stringToIntWithRadix(const char *s,
               int forceRadix, //!< if radix=0, autodetect
               bool *hasError, //!< If nonzero, set to whether there was an error or not
               const char **endOfInteger //!<  If nonzero, this is set to the point at which the integer finished in the string
               ) {
  while (isWhitespace(*s)) s++;

  bool isNegated = false;
  long long v = 0;
  if (*s == '-') {
    isNegated = true;
    s++;
  } else if (*s == '+') {
    s++;
  }

  const char *numberStart = s;
  if (endOfInteger) (*endOfInteger)=s;

  int radix = getRadix(&s, forceRadix, hasError);
  if (!radix) return 0;

  while (*s) {
    int digit = chtod(*s);
    if (digit<0 || digit>=radix)
      break;
    v = v*radix + digit;
    s++;
  }

  if (hasError)
    *hasError = s==numberStart; // we had an error if we didn't manage to parse any chars at all
  if (endOfInteger) (*endOfInteger)=s;

  if (isNegated) return -v;
  return v;
}
