bool isIDString(const char *s) {
  if (!isAlpha(*s))
    return false;
  while (*s) {
    if (!(isAlpha(*s) || isNumeric(*s)))
      return false;
    s++;
  }
  return true;
}
