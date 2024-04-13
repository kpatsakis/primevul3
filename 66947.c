bool jsvIsStringNumericInt(const JsVar *var, bool allowDecimalPoint) {
  assert(jsvIsString(var));
  JsvStringIterator it;
  jsvStringIteratorNewConst(&it, var, 0); // we know it's non const

  while (jsvStringIteratorHasChar(&it) && isWhitespace(jsvStringIteratorGetChar(&it)))
    jsvStringIteratorNext(&it);

  if (jsvStringIteratorGetChar(&it)=='-' || jsvStringIteratorGetChar(&it)=='+')
    jsvStringIteratorNext(&it);

  int radix = 0;
  if (jsvStringIteratorGetChar(&it)=='0') {
    jsvStringIteratorNext(&it);
    char buf[3];
    buf[0] = '0';
    buf[1] = jsvStringIteratorGetChar(&it);
    buf[2] = 0;
    const char *p = buf;
    radix = getRadix(&p,0,0);
    if (p>&buf[1]) jsvStringIteratorNext(&it);
  }
  if (radix==0) radix=10;

  int chars=0;
  while (jsvStringIteratorHasChar(&it)) {
    chars++;
    char ch = jsvStringIteratorGetChar(&it);
    if (ch=='.' && allowDecimalPoint) {
      allowDecimalPoint = false; // there can be only one
    } else {
      int n = chtod(ch);
      if (n<0 || n>=radix) {
        jsvStringIteratorFree(&it);
        return false;
      }
    }
    jsvStringIteratorNext(&it);
  }
  jsvStringIteratorFree(&it);
  return chars>0;
}
