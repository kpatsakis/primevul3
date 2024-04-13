bool jsvIsStringEqualOrStartsWithOffset(JsVar *var, const char *str, bool isStartsWith, size_t startIdx, bool ignoreCase) {
  if (!jsvHasCharacterData(var)) {
    return 0; // not a string so not equal!
  }

  JsvStringIterator it;
  jsvStringIteratorNew(&it, var, startIdx);
  if (ignoreCase) {
      while (jsvStringIteratorHasChar(&it) && *str &&
             jsvStringCharToLower(jsvStringIteratorGetChar(&it)) == jsvStringCharToLower(*str)) {
        str++;
        jsvStringIteratorNext(&it);
      }
  } else {
      while (jsvStringIteratorHasChar(&it) && *str &&
             jsvStringIteratorGetChar(&it) == *str) {
        str++;
        jsvStringIteratorNext(&it);
      }
  }
  bool eq = (isStartsWith && !*str) ||
            jsvStringIteratorGetChar(&it)==*str; // should both be 0 if equal
  jsvStringIteratorFree(&it);
  return eq;
}
