bool jsvIsStringNumericStrict(const JsVar *var) {
  assert(jsvIsString(var));
  JsvStringIterator it;
  jsvStringIteratorNewConst(&it, var, 0);  // we know it's non const
  bool hadNonZero = false;
  bool hasLeadingZero = false;
  int chars = 0;
  while (jsvStringIteratorHasChar(&it)) {
    chars++;
    char ch = jsvStringIteratorGetChar(&it);
    if (!isNumeric(ch)) {
      jsvStringIteratorFree(&it);
      return false;
    }
    if (!hadNonZero && ch=='0') hasLeadingZero=true;
    if (ch!='0') hadNonZero=true;
    jsvStringIteratorNext(&it);
  }
  jsvStringIteratorFree(&it);
  return chars>0 && (!hasLeadingZero || chars==1);
}
