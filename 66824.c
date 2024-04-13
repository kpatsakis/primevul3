void jsvAppendStringBuf(JsVar *var, const char *str, size_t length) {
  assert(jsvIsString(var));
  JsvStringIterator dst;
  jsvStringIteratorNew(&dst, var, 0);
  jsvStringIteratorGotoEnd(&dst);
  /* This isn't as fast as something single-purpose, but it's not that bad,
   * and is less likely to break :) */
  while (length) {
    jsvStringIteratorAppend(&dst, *(str++));
    length--;
  }
  jsvStringIteratorFree(&dst);
}
