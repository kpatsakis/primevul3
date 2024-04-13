void jsvGetLineAndCol(JsVar *v, size_t charIdx, size_t *line, size_t *col) {
  size_t x = 1;
  size_t y = 1;
  size_t n = 0;
  assert(line && col);

  JsvStringIterator it;
  jsvStringIteratorNew(&it, v, 0);
  while (jsvStringIteratorHasChar(&it)) {
    char ch = jsvStringIteratorGetChar(&it);
    if (n==charIdx) {
      jsvStringIteratorFree(&it);
      *line = y;
      *col = x;
      return;
    }
    x++;
    if (ch=='\n') {
      x=1; y++;
    }
    n++;
    jsvStringIteratorNext(&it);
  }
  jsvStringIteratorFree(&it);
  *line = y;
  *col = x;
}
