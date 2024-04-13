JsVar *jsvAsFlatString(JsVar *var) {
  if (jsvIsFlatString(var)) return jsvLockAgain(var);
  JsVar *str = jsvAsString(var, false);
  size_t len = jsvGetStringLength(str);
  JsVar *flat = jsvNewFlatStringOfLength((unsigned int)len);
  if (flat) {
    JsvStringIterator src;
    JsvStringIterator dst;
    jsvStringIteratorNew(&src, str, 0);
    jsvStringIteratorNew(&dst, flat, 0);
    while (len--) {
      jsvStringIteratorSetChar(&dst, jsvStringIteratorGetChar(&src));
      if (len>0) {
        jsvStringIteratorNext(&src);
        jsvStringIteratorNext(&dst);
      }
    }
    jsvStringIteratorFree(&src);
    jsvStringIteratorFree(&dst);
  }
  jsvUnLock(str);
  return flat;
}
