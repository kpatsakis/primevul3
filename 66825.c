void jsvAppendStringVar(JsVar *var, const JsVar *str, size_t stridx, size_t maxLength) {
  assert(jsvIsString(var));

  JsvStringIterator dst;
  jsvStringIteratorNew(&dst, var, 0);
  jsvStringIteratorGotoEnd(&dst);
  /* This isn't as fast as something single-purpose, but it's not that bad,
     * and is less likely to break :) */
  JsvStringIterator it;
  jsvStringIteratorNewConst(&it, str, stridx);
  while (jsvStringIteratorHasChar(&it) && (maxLength-->0)) {
    char ch = jsvStringIteratorGetChar(&it);
    jsvStringIteratorAppend(&dst, ch);
    jsvStringIteratorNext(&it);
  }
  jsvStringIteratorFree(&it);
  jsvStringIteratorFree(&dst);
}
