size_t jsvGetIndexFromLineAndCol(JsVar *v, size_t line, size_t col) {
  size_t x = 1;
  size_t y = 1;
  size_t n = 0;
  JsvStringIterator it;
  jsvStringIteratorNew(&it, v, 0);
  while (jsvStringIteratorHasChar(&it)) {
    char ch = jsvStringIteratorGetChar(&it);
    if ((y==line && x>=col) || y>line) {
      jsvStringIteratorFree(&it);
      return (y>line) ? (n-1) : n;
    }
    x++;
    if (ch=='\n') {
      x=1; y++;
    }
    n++;
    jsvStringIteratorNext(&it);
  }
  jsvStringIteratorFree(&it);
  return n;
}
