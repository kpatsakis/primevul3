int jsvGetStringIndexOf(JsVar *str, char ch) {
  JsvStringIterator it;
  jsvStringIteratorNew(&it, str, 0);
  while (jsvStringIteratorHasChar(&it)) {
    if (jsvStringIteratorGetChar(&it) == ch) {
      int idx = (int)jsvStringIteratorGetIndex(&it);
      jsvStringIteratorFree(&it);
      return idx;
    };
    jsvStringIteratorNext(&it);
  }
  jsvStringIteratorFree(&it);
  return -1;
}
